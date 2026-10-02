# Задача 4 — OpenMP: умножение больших матриц

## Описание
Программа умножает две квадратные матрицы `N×N`:
- **последовательно** (один поток);
- **параллельно** с заданным числом потоков.

Замеряется время выполнения, вычисляется ускорение (speedup), проверяется корректность результата.

Распараллелен **внешний цикл по `i`** (по строкам результата `C[i][*]`).
Это безопасно: каждый поток пишет в свою строку, зависимости по данным нет.

## Сборка

### MSVC (Developer Command Prompt)
```cmd
cl /openmp /O2 main.c
```

### GCC / Clang (Linux, macOS, MinGW)
```bash
gcc -fopenmp -O2 -o task4 main.c
```

## Запуск

### Синтаксис
```
task4.exe <N> <num_threads>
```

- `N` — размер матрицы (`N × N`).
- `num_threads` — число потоков (`0` = только последовательная версия).

### Windows (MSVC)
```cmd
task4.exe 500 0
task4.exe 500 2
task4.exe 500 4
task4.exe 500 8
```

### Linux / macOS / MinGW
```bash
./task4 500 0
./task4 500 2
./task4 500 4
./task4 500 8
```

## Пример вывода

### Только последовательная версия
```
Matrix size: 500 x 500
Threads: 0

Sequential: 1.234567 sec
```

### 4 потока
```
Matrix size: 500 x 500
Threads: 4

Sequential: 1.234567 sec
Parallel (4 threads): 0.345678 sec
Speedup: 3.571
Result check: OK
```
