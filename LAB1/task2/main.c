#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char* argv[])
{
    // Программа должна запускаться с двумя аргументами - число потоков и размер массива (N)
    if (argc != 3) {
        fprintf(stderr, "Use two argument\n");
        return 1;
    }

    int num_threads = atoi(argv[1]);
    int N = atoi(argv[2]);

    // Проверка: если число потоков не положительное - вывод сообщения
    if (num_threads <= 0) {
        fprintf(stderr, "The number of threads must be positive\n");
        return 1;
    }
    // Проверка чтобы в массиве было хотя бы 3 элемента
    if (N < 3) {
        fprintf(stderr, "N must be at least 3\n");
        return 1;
    }

    // Выделение памяти под массивы
    double* a = (double*)malloc(N * sizeof(double));
    double* b = (double*)malloc(N * sizeof(double));

    // Проверка успешного выделения памяти
    if (a == NULL || b == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(a);
        free(b);
        return 1;
    }

    for (int i = 0; i < N; i++) {
        a[i] = (double)i;
    }

    omp_set_num_threads(num_threads);
    int i;
    // Параллельный цикл
#pragma omp parallel for schedule(runtime)
    for (i = 1; i < N - 1; i++) {
        b[i] = (a[i - 1] + a[i] + a[i + 1]) / 3.0;
    }

    // Крайние элементы оставляем
    b[0] = a[0];
    b[N - 1] = a[N - 1];

    // Печать первых 10 и последних 10 элементов
    printf("First 10 elements of b:\n");
    for (int i = 0; i < 10 && i < N; i++) {
        printf("b[%d] = %.3f\n", i, b[i]);
    }
    printf("...\n");

    printf("Last 10 elements of b:\n");
    for (int i = (N - 10 > 0 ? N - 10 : 0); i < N; i++) {
        printf("b[%d] = %.3f\n", i, b[i]);
    }

    free(a);
    free(b);
    return 0;
}
