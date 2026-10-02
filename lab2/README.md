# Задача 2 — OpenMP: средние значения массива

## Описание
Программа создаёт массив `a` из `N` элементов, где `a[i] = i`.
Затем параллельно вычисляет массив `b`, где
`b[i] = (a[i-1] + a[i] + a[i+1]) / 3.0` для внутренних элементов.
Крайние элементы копируются без изменений.

Число потоков и размер массива передаются через командную строку.
Тип распределения работ задаётся переменной окружения `OMP_SCHEDULE`
благодаря директиве `schedule(runtime)`.

## Сборка

### MSVC (Developer Command Prompt)
```cmd
cl /openmp /O2 main.c
```

### GCC / Clang (Linux, macOS, MinGW)
```bash
gcc -fopenmp -O2 -o task2 main.c
```

## Запуск

### Синтаксис
```
task2.exe <num_threads> <N>
```

- `num_threads` — число потоков (положительное целое).
- `N` — размер массива (≥ 3).

### Windows (MSVC)
```cmd
set OMP_SCHEDULE=static
task2.exe 8 16000
```

### Linux / macOS / MinGW
```bash
OMP_SCHEDULE="static" ./task2 8 16000
```

## Примеры запуска с разными типами распределения

### Windows
```cmd
set OMP_SCHEDULE=static
task2.exe 8 16000

set OMP_SCHEDULE=static,1000
task2.exe 8 16000

set OMP_SCHEDULE=dynamic
task2.exe 8 16000

set OMP_SCHEDULE=dynamic,1000
task2.exe 8 16000

set OMP_SCHEDULE=guided
task2.exe 8 16000

set OMP_SCHEDULE=guided,1000
task2.exe 8 16000
```

### Linux / macOS / MinGW
```bash
OMP_SCHEDULE="static" ./task2 8 16000
OMP_SCHEDULE="static,1000" ./task2 8 16000
OMP_SCHEDULE="dynamic" ./task2 8 16000
OMP_SCHEDULE="dynamic,1000" ./task2 8 16000
OMP_SCHEDULE="guided" ./task2 8 16000
OMP_SCHEDULE="guided,1000" ./task2 8 16000
```

## Ожидаемый результат
На экран выводятся первые и последние 10 элементов массива `b`:

```
First 10 elements of b:
b[0] = 0.000
b[1] = 1.000
b[2] = 2.000
...
b[9] = 9.000
...
Last 10 elements of b:
b[15990] = 15990.000
b[15991] = 15991.000
...
b[15999] = 15999.000
```
