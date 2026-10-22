# Function Table Calculator (Variant 3)

Laboratory works of the discipline «Software Engineering», NTU “KhPI”, group KN-226e, Budkov O. Yu.

The console program calculates the function

```
y = 1/(2x) + Π (i + x),                     i = 1..n-2,             if x < 3
y = Π (j = 1..n) Σ (i = 1..n+3) (i + 1/(4j²)),                       if x ≥ 3
```

for every x from the start value to the end value with the given step and prints a table of x and y.

## How to run

- **Visual Studio:** create an empty C++ console project, add `src/main.cpp`, build and run (Ctrl + F5).
- **g++:** `g++ -std=c++17 -o lab2 src/main.cpp`, then `./lab2`.

Input rules: the end value is not smaller than the start value, the step is greater than 0
(at most 1000 rows), n is a whole number from 4 to 1000. At x = 0 the program prints “not defined”.
The file for option 2 contains four numbers: `start end step n` (example: `src/input.txt`).

## Repository structure

| Folder | Content |
| --- | --- |
| `src/` | C++ program (Lab 2) and an example input file |
| `docs/lab1/` | Lab 1 report, Activity and Use Case diagrams |
| `docs/lab2/` | Lab 2 report, wireframe and screen transitions |
| `docs/lab3/` | Lab 3 report |
| `tools/` | script that creates labels, milestones and issues on GitHub |

## Project plan

The project follows an iterative model: every laboratory work is one iteration (milestone)
with one epic task and several subtasks.

| Milestone | Epic |
| --- | --- |
| Iteration 1 – Domain modelling (Lab 1) | Model the domain of Variant 3 with UML |
| Iteration 2 – Coding of domain (Lab 2) | Implement the function table calculator in C++ |
| Iteration 3 – Planning of system activities (Lab 3) | Set up project tracking on GitHub |
| Iteration 4 – Software requirements specification (Lab 4) | Write the SRS of the program |
| Iteration 5 – Testing of code (Lab 5) | Refactor the program and cover it with unit tests |
