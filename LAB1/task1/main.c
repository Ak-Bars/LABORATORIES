#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char* argv[])
{
    // Проверка: если введено больше 1 аргумента - вывод сообщения об ошибке
    // Программа должна запускаться ровно с одним аргументом
    if (argc != 2) {
        fprintf(stderr, "Use only one argument\n");
        return 1;
    }

    // Проверка: если число потоков не положительное - вывод сообщения об ошибке
    int num_threads = atoi(argv[1]);
    if (num_threads <= 0) {
        fprintf(stderr, "The number of threads must be positive\n");
        return 1;
    }

    omp_set_num_threads(num_threads);
    
#pragma omp parallel
    {
        int id = omp_get_thread_num();
        int total = omp_get_num_threads();
        printf("Hello World! Thread %d of %d\n", id, total);
    }

    return 0;
}
