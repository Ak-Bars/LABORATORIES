# Задача 5 — Геометрический центр точек в 3D

## Описание
Программа читает из файла координаты точек в 3D (`x, y, z`),
вычисляет геометрический центр (среднее по каждой координате).

Реализованы две версии:
- **A: распараллеливание цикла** (`#pragma omp parallel for` + `reduction`);
- **B: функциональная декомпозиция** (`#pragma omp parallel sections`).

## Сборка

### MSVC (Developer Command Prompt)
```cmd
cl /openmp /O2 main.c
```

### GCC / Clang (Linux, macOS, MinGW)
```bash
gcc -fopenmp -O2 -o task5 main.c -lm
```

## Запуск

### Синтаксис
```
task5.exe <filename> <num_threads>
```

- `filename` — файл с точками (по 3 числа на строку).
- `num_threads` — число потоков (положительное).

### Windows (MSVC)
```cmd
task5.exe points.txt 4
```

### Linux / macOS / MinGW
```bash
./task5 points.txt 4
```

## Формат файла `points.txt`
```
1.0 2.0 3.0
4.0 5.0 6.0
7.0 8.0 9.0
...
```

## Пример вывода
```
File: points.txt
Points: 1000000
Threads: 4

Read time: 1.598866 sec

Parallel for
Center: (-0.522536, -0.455024, -0.430938)
Time: 0.001307 sec

Functional decomposition
Center: (-0.522536, -0.455024, -0.430938)
Time: 0.002722 sec

Result check: OK
```

## Разница между версиями
| | Версия A (`parallel for`) | Версия B (`sections`) |
|---|---|---|
| Что делится | Данные (точки) | Функции (суммы x/y/z) |
| Поток 0 | Считает все суммы для своей части точек | Только сумму `x` |
| Поток 1 | То же для своей части | Только сумму `y` |
| Поток 2 | То же для своей части | Только сумму `z` |
| Максимум потоков | Любое | **3** (по числу секций) |
| Когда хорошо | Много точек, мало функций | Мало функций, много данных |
