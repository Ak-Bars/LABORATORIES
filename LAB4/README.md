# Задача 6 — Глобальная сумма через функциональную декомпозицию

## Описание
Программа читает из файла координаты точек (`x, y, z`)
и вычисляет значение по формуле:

```
(Σx + Σy + Σz) / 3N
```

где `N` — количество точек.

Используется **функциональная декомпозиция** (`#pragma omp parallel sections`)
и **критические секции** (`#pragma omp critical`):
- поток 0 считает `Σx`,
- поток 1 считает `Σy`,
- поток 2 считает `Σz`,
- результаты складываются в глобальную сумму через `critical`.

## Сборка

### MSVC (Developer Command Prompt)
```cmd
cl /openmp /O2 main.c
```

### GCC / Clang (Linux, macOS, MinGW)
```bash
gcc -fopenmp -O2 -o task6 main.c -lm
```

## Запуск

### Синтаксис
```
task6.exe <filename> <num_threads>
```

- `<filename>` — файл с точками (по 3 числа на строку);
- `<num_threads>` — число потоков (положительное).

### Windows (MSVC)
```cmd
task6.exe points.txt 4
```

### Linux / macOS / MinGW
```bash
./task6 points.txt 4
```

## Формат файла `points.txt`
```
1.0 2.0 3.0
4.0 5.0 6.0
...
```

## Пример вывода
```
File: points.txt
Points: 1000000
Threads: 4

Read time: 1.737859 sec

Functional decomposition
Global sum / 3N = -0.469499
Time: 0.002612 sec
```
