# Entry-test
#### GL C/Embedded BaseCamp Entry Test

-----

## Task-1

I would like to make a little comment regarding Task#1. 

In the task is given that Giant could step over each K-th stair.

In this case, the answer to the question of how many stairs will Giant step in total is N - N/K. 

(He steps over each K-th stair, then in total he steps over N/K stairs. That means that he steps N - N/K stairs.)


But, according to the given examples, he steps over K stairs on each step. In my code, I provide solution for this case.

---------

### Instructions & examples

- Task-1
```bash
g++ task1.cpp -o steps

./steps $N $K
```
- Task-2
```bash
g++ task2.cpp -o cables

./steps $N $D
```
- Task-3
```bash
g++ task3.cpp -o ls
```
```bash
./ls -la

./ls -l

./ls -a
```

- Task-4
```bash
g++ task4.cpp -o extract

./extract file.img somedir
```

