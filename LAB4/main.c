#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

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

double center_functional_critical(Point* points, int n)
{
    double global_sum = 0.0;

#pragma omp parallel sections shared(points, n, global_sum)
    {
#pragma omp section
        {
            double sx = 0.0;
            for (int i = 0; i < n; i++) {
                sx += points[i].x;
            }

#pragma omp critical
            {
                global_sum += sx;
            }
        }

#pragma omp section
        {
            double sy = 0.0;
            for (int i = 0; i < n; i++) {
                sy += points[i].y;
            }

#pragma omp critical
            {
                global_sum += sy;
            }
        }

#pragma omp section
        {
            double sz = 0.0;
            for (int i = 0; i < n; i++) {
                sz += points[i].z;
            }

#pragma omp critical
            {
                global_sum += sz;
            }
        }
    }

    return global_sum / (3.0 * n);
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

    omp_set_num_threads(num_threads);

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

    t_start = omp_get_wtime();
    double result = center_functional_critical(points, n);
    double t_calc = omp_get_wtime() - t_start;

    printf("Functional decomposition\n");
    printf("Global sum / 3N = %.6f\n", result);
    printf("Time: %.6f sec\n", t_calc);

    free(points);
    return 0;
}
