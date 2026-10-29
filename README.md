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

## How we work: Scrum

The work is planned with Scrum — fixed three-week sprints, each with a sprint goal and a working increment.

- **Product Backlog** — [issues](https://github.com/oleksiibudkov/SE-Lab-Var3/issues) labelled `user story`:
  “As a …, I want …, so that …”, with acceptance criteria, story points (1, 2, 3, 5, 8) and a MoSCoW priority.
- **Sprint Backlog** — the `task` issues of the stories in the current sprint; every task is a sub-issue of its story.
- **Sprints** — [milestones](https://github.com/oleksiibudkov/SE-Lab-Var3/milestones) with the sprint goal in the description.
- **Scrum board** — [GitHub Project](https://github.com/users/oleksiibudkov/projects/1):
  Product Backlog → Sprint Backlog → In Progress → Done.

| Sprint | Dates | Sprint goal | Story points |
| --- | --- | --- | --- |
| Sprint 1 | 7 Sep – 25 Sep | the behaviour of the calculator is agreed and modelled in UML | 8 ✔ |
| Sprint 2 | 28 Sep – 16 Oct | a working console calculator: input, table, saving | 18 ✔ |
| Sprint 3 | 19 Oct – 6 Nov | the work is tracked with Scrum and the code is split into modules | 10 |
| Sprint 4 | 9 Nov – 27 Nov | the Software Requirements Specification | 8 |
| Sprint 5 | 30 Nov – 18 Dec | errors never crash the program, unit tests protect the results | 13 |

**Definition of Done:** the acceptance criteria are met, the code builds without warnings, `make test` passes,
the change is merged into `main` through a pull request, and the documentation is updated.

The labels, sprints, stories, tasks and the board are created by
`bash tools/github_setup.sh oleksiibudkov/SE-Lab-Var3 oleksiibudkov` (GitHub CLI).
