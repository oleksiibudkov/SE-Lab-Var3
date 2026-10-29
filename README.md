# Function Table Calculator (Variant 3)

Laboratory works of the discipline «Software Engineering», NTU “KhPI”, group KN-226e, Budkov O. Yu.

The console program calculates the function

```
y = 1/(2x) + Π (i + x),                     i = 1..n-2,             if x < 3
y = Π (j = 1..n) Σ (i = 1..n+3) (i + 1/(4j²)),                       if x ≥ 3
```

for every x from the start value to the end value with the given step and prints a table of x and y.

## Build and run

| Tool | Commands |
| --- | --- |
| Terminal (g++ / clang++) | `make` — build, `make run` — run, `make test` — regression test |
| Visual Studio 2019+ | **File → Open → Folder** with this repository (it reads `CMakeLists.txt`), then **Debug → Start** |
| CMake | `cmake -S . -B build && cmake --build build`, then `./build/calculator` |

Input rules: the end value is not smaller than the start value, the step is greater than 0
(at most 1000 rows), n is a whole number from 4 to 1000. At x = 0 the program prints “not defined”.
The file for option 2 contains four numbers: `start end step n` (example: `data/input.txt`).

## Code structure

The program is split into three layers; a layer uses only the layers below it.

```
src/
├── main.cpp                  entry point: the main menu loop
├── ui/                       screens
│   └── screens.h/.cpp        main menu, result table, help
├── io/                       input and output
│   ├── console.h/.cpp        reading numbers, menu options, y/n answers
│   ├── data_input.h/.cpp     data from the keyboard or from a file
│   └── table_printer.h/.cpp  table of x and y on the screen or in a file
└── core/                     logic without input/output
    ├── input_data.h          InputData and the limits MAX_POINTS, MAX_N
    ├── calculator.h/.cpp     calcY(x, n) — the function of Variant 3
    ├── validation.h/.cpp     checks of the end value, the step and n
    └── format.h/.cpp         numbers and data as text
```

All code is in the namespace `ftc` (Function Table Calculator). `tests/regression/` runs the program
with recorded scenarios and compares the output with the reference of the original single-file version.

## Repository structure

| Folder | Content |
| --- | --- |
| `src/` | C++ program, split into modules (see above) |
| `data/` | example input file |
| `tests/` | regression test |
| `docs/lab1/` | Lab 1 report, Activity and Use Case diagrams |
| `docs/lab2/` | Lab 2 report, wireframe and screen transitions |
| `docs/lab3/` | Lab 3 report |
| `tools/` | script that creates the Scrum backlog, sprints and board on GitHub |

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

Lab 3: project plan – see Milestones and Issues.
