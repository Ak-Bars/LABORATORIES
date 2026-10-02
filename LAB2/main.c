#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Выделение памяти под матрицу N x N
 double** alloc_matrix(int N)
{
    double** m = (double**)malloc(N * sizeof(double*));
    if (m == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    for (int i = 0; i < N; i++) {
        m[i] = (double*)malloc(N * sizeof(double));
        if (m[i] == NULL) {
            fprintf(stderr, "Memory allocation failed (row)\n");
            exit(1);
        }
    }
    return m;
}

// Освобождение памяти матрицы
void free_matrix(double** m, int N)
{
    for (int i = 0; i < N; i++) {
        free(m[i]);
    }
    free(m);
}

// Инициализация матрицы
void init_matrix(double** m, int N)
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            m[i][j] = (double)(i + j) / N;
        }
    }
}

// Последовательное умножение матриц: C = A * B
void multiply_sequential(double** A, double** B, double** C, int N)
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0.0;
            for (int k = 0; k < N; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

// Параллельное умножение матриц: C = A * B
// Распараллелен внешний цикл по i (по строкам результата)
void multiply_parallel(double** A, double** B, double** C, int N, int num_threads)
{
    omp_set_num_threads(num_threads);

    int i, j, k;
#pragma omp parallel for private(j, k) shared(A, B, C, N)
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            double sum = 0.0;
            for (k = 0; k < N; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

//Проверка: сравниваем элементы двух матриц
int matrices_equal(double** A, double** B, int N, double eps)
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double diff = A[i][j] - B[i][j];
            if (diff < 0) diff = -diff;
            if (diff > eps) return 0;
        }
    }
    return 1;
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Use two argument, N - matrix size and num_threads - number of threads\n");
        return 1;
    }

    int N = atoi(argv[1]);
    int num_threads = atoi(argv[2]);

    if (N <= 0) {
        fprintf(stderr, "N must be positive\n");
        return 1;
    }
    if (num_threads < 0) {
        fprintf(stderr, "num_threads must be >= 0\n");
        return 1;
    }

    printf("Matrix size: %d x %d\n", N, N);
    printf("Threads: %d\n", num_threads);

    double** A = alloc_matrix(N);
    double** B = alloc_matrix(N);
    double** C_seq = alloc_matrix(N);
    double** C_par = alloc_matrix(N);

    init_matrix(A, N);
    init_matrix(B, N);

    double t_start = omp_get_wtime();
    multiply_sequential(A, B, C_seq, N);
    double t_seq = omp_get_wtime() - t_start;

    printf("Sequential: %.6f sec\n", t_seq);

    if (num_threads > 0) {
        t_start = omp_get_wtime();
        multiply_parallel(A, B, C_par, N, num_threads);
        double t_par = omp_get_wtime() - t_start;

        printf("Parallel (%d threads): %.6f sec\n", num_threads, t_par);
        printf("Speedup: %.3f\n", t_seq / t_par);

        if (matrices_equal(C_seq, C_par, N, 1e-9)) {
            printf("Result check: OK\n");
        }
        else {
            printf("Result check: FAILED\n");
        }
    }

    free_matrix(A, N);
    free_matrix(B, N);
    free_matrix(C_seq, N);
    free_matrix(C_par, N);

    return 0;
}
