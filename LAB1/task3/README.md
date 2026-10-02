Задача 3 — OpenMP: обратный порядок потоков

Модификация Задачи 1: потоки печатают свои идентификаторы
в обратном порядке (от N-1 до 0).

Реализовано 5 способов:

1. ordered — упорядоченное выполнение через `#pragma omp ordered`
2. atomic — активное ожидание
3. chain — цепочка флагов
4. barrier — барьер + single
5. critical — критическая секция

Сборка

MSVC (Developer Command Prompt)
```cmd
cl /openmp /O2 main.c
```

GCC / Clang (Linux, macOS, MinGW)
```bash
gcc -fopenmp -O2 -o task3 main.c
```

Запуск

Синтаксис
```
task3.exe <num_threads> [method]
```

- num_threads — число потоков (положительное целое).
- metho` — способ: ordered, atomic, chain, barrier, critical, all.
  По умолчанию — al` (все 5 методов подряд).

Все 5 методов подряд
```cmd
task3.exe 8 all
```

Конкретный метод
```cmd
task3.exe 8 ordered
task3.exe 8 atomic
task3.exe 8 chain
task3.exe 8 barrier
task3.exe 8 critical
```

Linux / macOS / MinGW
```bash
./task3 8 all
./task3 8 ordered
./task3 8 atomic
./task3 8 chain
./task3 8 barrier
./task3 8 critical
```

Ожидаемый результат
Во всех методах порядок строк строго от `N-1` до `0`:
```
Thread 7 of 8: Hello World
Thread 6 of 8: Hello World
Thread 5 of 8: Hello World
Thread 4 of 8: Hello World
Thread 3 of 8: Hello World
Thread 2 of 8: Hello World
Thread 1 of 8: Hello World
Thread 0 of 8: Hello World
```
