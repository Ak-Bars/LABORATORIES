# Задача 1 — OpenMP: Hello World из каждого потока

## Описание
Программа создаёт параллельную область из `N` потоков.
Каждый поток печатает свой идентификатор, общее количество потоков
и строку `Hello World!`.

Число потоков передаётся через командную строку.

## Сборка

### MSVC (Developer Command Prompt)
```cmd
cl /openmp /O2 main.c
```

### GCC / Clang (Linux, macOS, MinGW)
```bash
gcc -fopenmp -O2 -o task1 main.c
```

## Запуск

### Синтаксис
```
task1.exe <num_threads>
```

- `num_threads` — число потоков (положительное целое).

### Windows (MSVC)
```cmd
task1.exe 8
```

### Linux / macOS / MinGW
```bash
./task1 8
```

## Ожидаемый результат
Программа печатает 8 строк с идентификаторами от 0 до 7:

```
Hello World! Thread 0 of 8
Hello World! Thread 3 of 8
Hello World! Thread 1 of 8
Hello World! Thread 2 of 8
Hello World! Thread 5 of 8
Hello World! Thread 4 of 8
Hello World! Thread 6 of 8
Hello World! Thread 7 of 8
```
