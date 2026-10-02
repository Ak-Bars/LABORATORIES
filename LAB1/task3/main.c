#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

void print_thread(int id, int total)
{
    printf("Hello World! Thread %d of %d\n", id, total);
}

//  Способ 1: Ordered
// Каждый поток выполняется параллельно, но блок ordered выполняется в порядке возрастания i
void method_ordered(int num_threads)
{
    printf("\nMethod 1: ordered\n");

    omp_set_num_threads(num_threads);

#pragma omp parallel
    {
        int total = omp_get_num_threads();
        int i;
#pragma omp for ordered
        for (i = 0; i < total; i++) {
#pragma omp ordered
            print_thread(total - 1 - i, total);
        }
    }
}

// Способ 2: Atomic Spinlock (Active Waiting) - Активное ожидание
// Используется атомарный счетчик.Каждый поток "крутится" в цикле, пока не придет его очередь.
// Потоки крутятся вхолостую
void method_atomic(int num_threads)
{
    printf("\nMethod 2: Активное ожидание\n");

    omp_set_num_threads(num_threads);

    int turn = 0;

#pragma omp parallel
    {
        int id = omp_get_thread_num();
        int total = omp_get_num_threads();

        // Поток с id = total-1 печатает первым (turn = 0),
        // поток с id = 0 печатает последним (turn = total-1)
        int my_turn = total - 1 - id;

        while (1) {
            int current;
#pragma omp atomic read
            current = turn;

            if (current == my_turn) {
                print_thread(id, total);
#pragma omp atomic write
                turn = my_turn + 1;
                break;
            }
        }
    }
}

// Способ 3: Channel Chain (цепочка каналов)
// Поток i ждет пока поток i+1 не напечатает, после чего начинает свою печать
 void method_chain(int num_threads)
{
    printf("\nMethod 3:Цепочка каналов\n");

    omp_set_num_threads(num_threads);

    // Обнуляем память
    int* flags = (int*)calloc(num_threads + 1, sizeof(int));
    if (flags == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    // Последний поток печатает сразу
    flags[num_threads] = 1;

#pragma omp parallel
    {
        int id = omp_get_thread_num();
        int total = omp_get_num_threads();

        // Ждём разрешения от "следующего" потока
        while (1) {
            int ready;
#pragma omp atomic read
            ready = flags[id + 1];
            if (ready == 1) break;
        }

        print_thread(id, total);

        // Разрешаем печатать "предыдущему" потоку
#pragma omp atomic write
        flags[id] = 1;
    }

    free(flags);
}

// Способ 4: Barrier + Single - Барьер + центральный координатор
// Все потоки записывают свои ID в общий массив, барьер ждет пока все допишут, 
// потом печатает массив
void method_barrier_single(int num_threads)
{
    printf("\nMethod 4: Barrier + Single\n");

    omp_set_num_threads(num_threads);

    int* ids = (int*)malloc(num_threads * sizeof(int));
    if (ids == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

#pragma omp parallel
    {
        int id = omp_get_thread_num();
        int total = omp_get_num_threads();

        ids[id] = id;

#pragma omp barrier

#pragma omp single
        {
            for (int i = total - 1; i >= 0; i--) {
                print_thread(ids[i], total);
            }
        }
    }

    free(ids);
}

// Способ 5: Critical (критическая секция)
// Каждый поток по очереди заходит в critical и проверяет, его ли очередь.
// В отличие от atomic, здесь используется блокировка — поток "засыпает", ожидая освобождения секции.
void method_critical(int num_threads)
{
    printf("\nMethod 5: Critical Section\n");

    omp_set_num_threads(num_threads);

    int turn = 0;

#pragma omp parallel
    {
        int id = omp_get_thread_num();
        int total = omp_get_num_threads();

        int my_turn = total - 1 - id;

        while (1) {
            int can_print = 0;

#pragma omp critical
            {
                if (turn == my_turn) {
                    print_thread(id, total);
                    turn = my_turn + 1;
                    can_print = 1;
                }
            }

            if (can_print) break;
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <num_threads> [method]\n", argv[0]);
        fprintf(stderr, "Methods: ordered | atomic | chain | barrier | critical | all\n");
        return 1;
    }

    int num_threads = atoi(argv[1]);
    if (num_threads <= 0) {
        fprintf(stderr, "The number of threads must be positive\n");
        return 1;
    }

    const char* method = (argc >= 3) ? argv[2] : "all";

    if (strcmp(method, "ordered") == 0) {
        method_ordered(num_threads);
    }
    else if (strcmp(method, "atomic") == 0) {
        method_atomic(num_threads);
    }
    else if (strcmp(method, "chain") == 0) {
        method_chain(num_threads);
    }
    else if (strcmp(method, "barrier") == 0) {
        method_barrier_single(num_threads);
    }
    else if (strcmp(method, "critical") == 0) {
        method_critical(num_threads);
    }
    else if (strcmp(method, "all") == 0) {
        method_ordered(num_threads);
        method_atomic(num_threads);
        method_chain(num_threads);
        method_barrier_single(num_threads);
        method_critical(num_threads);
    }
    else {
        fprintf(stderr, "Unknown method: %s\n", method);
        fprintf(stderr, "Methods: ordered | atomic | chain | barrier | critical | all\n");
        return 1;
    }

    return 0;
}
