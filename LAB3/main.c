#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

typedef struct {
    double x;
    double y;
    double z;
} Point;

Point* read_points(const char* filename, int* n)
{
    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        exit(1);
    }

    int capacity = 10000;
    int count = 0;
    Point* points = (Point*)malloc(capacity * sizeof(Point));
    if (points == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(f);
        exit(1);
    }

    double x, y, z;
    while (fscanf(f, "%lf %lf %lf", &x, &y, &z) == 3) {
        if (count == capacity) {
            capacity *= 2;
            points = (Point*)realloc(points, capacity * sizeof(Point));
            if (points == NULL) {
                fprintf(stderr, "Memory reallocation failed\n");
                fclose(f);
                exit(1);
            }
        }
        points[count].x = x;
        points[count].y = y;
        points[count].z = z;
        count++;
    }

    fclose(f);
    *n = count;
    return points;
}

// Распараллеливание цикла
void center_parallel_for(Point* points, int n, int num_threads,
    double* cx, double* cy, double* cz)
{
    double sx = 0.0, sy = 0.0, sz = 0.0;

    omp_set_num_threads(num_threads);

    int i;
#pragma omp parallel for reduction(+:sx, sy, sz) private(i)
    for (i = 0; i < n; i++) {
        sx += points[i].x;
        sy += points[i].y;
        sz += points[i].z;
    }

    *cx = sx / n;
    *cy = sy / n;
    *cz = sz / n;
}

// Функциональная декомпозиция
void center_functional(Point* points, int n,
    double* cx, double* cy, double* cz)
{
    double sx = 0.0, sy = 0.0, sz = 0.0;

#pragma omp parallel sections shared(points, n, sx, sy, sz)
    {
#pragma omp section
        {
            for (int i = 0; i < n; i++) {
                sx += points[i].x;
            }
        }

#pragma omp section
        {
            for (int i = 0; i < n; i++) {
                sy += points[i].y;
            }
        }

#pragma omp section
        {
            for (int i = 0; i < n; i++) {
                sz += points[i].z;
            }
        }
    }

    *cx = sx / n;
    *cy = sy / n;
    *cz = sz / n;
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <num_threads>\n", argv[0]);
        return 1;
    }

    const char* filename = argv[1];
    int num_threads = atoi(argv[2]);

    if (num_threads <= 0) {
        fprintf(stderr, "num_threads must be positive\n");
        return 1;
    }

    int n = 0;
    double t_start = omp_get_wtime();
    Point* points = read_points(filename, &n);
    double t_read = omp_get_wtime() - t_start;

    if (n == 0) {
        fprintf(stderr, "No points read from file\n");
        free(points);
        return 1;
    }

    printf("File: %s\n", filename);
    printf("Points: %d\n", n);
    printf("Threads: %d\n\n", num_threads);
    printf("Read time: %.6f sec\n\n", t_read);

    // Распараллеливание цикла
    double cx_a, cy_a, cz_a;
    t_start = omp_get_wtime();
    center_parallel_for(points, n, num_threads, &cx_a, &cy_a, &cz_a);
    double t_a = omp_get_wtime() - t_start;

    printf("Parallel for\n");
    printf("Center: (%.6f, %.6f, %.6f)\n", cx_a, cy_a, cz_a);
    printf("Time: %.6f sec\n\n", t_a);

    // Функциональная декомпозиция
    double cx_b, cy_b, cz_b;
    t_start = omp_get_wtime();
    center_functional(points, n, &cx_b, &cy_b, &cz_b);
    double t_b = omp_get_wtime() - t_start;

    printf("Functional decomposition\n");
    printf("Center: (%.6f, %.6f, %.6f)\n", cx_b, cy_b, cz_b);
    printf("Time: %.6f sec\n\n", t_b);

    double eps = 1e-9;
    if (fabs(cx_a - cx_b) < eps &&
        fabs(cy_a - cy_b) < eps &&
        fabs(cz_a - cz_b) < eps) {
        printf("Result check: OK\n");
    }
    else {
        printf("Result check: FAILED\n");
    }

    free(points);
    return 0;
}
