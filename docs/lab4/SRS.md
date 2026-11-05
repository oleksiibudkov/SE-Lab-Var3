# Software Requirements Specification

**Product:** Function Table Calculator (Variant 3)

**Author:** Budkov O. Yu., group KN-226e, NTU “KhPI”

**Version:** 1.0, November 2026

**User story:** US-10 (#71), Sprint 4

## 1. Purpose

This document describes what the Function Table Calculator must do. The customer uses it to accept the product, and the developer uses it to build and test the product. Every requirement has an identifier and a way to verify it.

The requirements of the product are the user stories of the Product Backlog on GitHub. Each functional requirement below belongs to one user story, so the document and the Scrum board use the same numbers.

### 1.1 Definitions

| Term | Meaning |
| --- | --- |
| x | the argument of the function |
| y(x) | the value of the function of Variant 3 for one x |
| start, end, step | the first value of x, the last value of x and the distance between two values of x |
| n | the upper limit of the counters in the formula, a whole number |
| Result table | the list of pairs x and y for every x from start to end |
| User story | a requirement written as “As a …, I want …, so that …” |
| Story points (SP) | the relative size of a user story: 1, 2, 3, 5 or 8 |
| MoSCoW | a priority: Must, Should or Could have |
| Regression scenario | a recorded input with the expected output, in the folder `tests/regression` |

### 1.2 System overview

The Function Table Calculator is a console program in C++. It calculates this function for every x from the start value to the end value with the given step:

```
y = 1/(2x) + Π (i + x),                  i = 1..n-2,              if x < 3
y = Π (j = 1..n) Σ (i = 1..n+3) (i + 1/(4j²)),                    if x ≥ 3
```

The user enters the data from the keyboard or loads it from a file, sees the result table and can save it to a file. The formula sets two limits: n must be greater than 3, because the upper limit n-2 of the first product must be greater than 1, and x = 0 has no value, because 1/(2x) divides by zero. The second branch does not contain x, so for the same n it gives the same y for every x ≥ 3.

### 1.3 References

1. Melnyk K. Methodical recommendation to Software Engineering laboratory practice. NTU “KhPI”, 2026.
2. IEEE Std 830-1998. Recommended Practice for Software Requirements Specifications.
3. Budkov O. Yu. Reports of laboratory works 1–3, folder `docs` of the repository.
4. Repository of the project: https://github.com/oleksiibudkov/SE-Lab-Var3
5. Scrum board of the project: https://github.com/users/oleksiibudkov/projects/1

## 2. Overall description

### 2.1 Product perspective

The program is a standalone application. It is not a part of a larger system.

| Interface | Description |
| --- | --- |
| System interfaces | none: the program works alone |
| User interfaces | a text menu in the console, in English; the user types the number of an option and presses Enter |
| Hardware interfaces | keyboard and screen through the console, no direct access to hardware |
| Software interfaces | the C++ standard library and the file system of the operating system |
| Communication interfaces | none: the program does not use a network |
| Memory constraints | the program keeps only the current data and prints the table row by row, so the table is not stored in memory |

### 2.2 Design constraints

**Operations.** The user starts the program, works in the main menu and leaves it with option 0. The end of input (Ctrl+D or Ctrl+Z) also ends the program.

**Site adaptation requirements.** The program needs no installation or settings. Files are read from and written to the folder where the program runs.

The code is split into the layers `core`, `io` and `ui`, and a layer uses only the layers below it. The `core` layer has no console input or output.

### 2.3 Product functions

- Enter the data from the keyboard, with a check of every value.
- Load the data from a text file.
- Calculate y for every x and show the result table.
- Save the result table to a file.
- Show the main menu and the help.

### 2.4 User characteristics

The users are students and teachers. They know the formula and can work with a console. They need no programming knowledge. The program explains every value and every error.

### 2.5 Constraints, assumptions and dependencies

- n is a whole number from 4 to 1000.
- The table has at most 1000 rows.
- Numbers are typed with a dot: 0.5, not 0,5.
- A C++17 compiler is needed to build the program.
- The results are double precision numbers, so very big values can exceed the range of the type.

## 3. Specific requirements

### 3.1 External interface requirements

### 3.2 Functional requirements

### 3.3 Performance requirements

### 3.4 Logical database requirements

### 3.5 Software system attributes

### 3.6 Environment characteristics

## 4. Traceability
