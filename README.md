# Multi-Process Prime Checker in C

A multi-process prime-number checker implemented in **C**, demonstrating Unix system programming concepts such as process creation, inter-process communication (IPC), pipes, low-level file I/O, and process synchronisation.

The program uses `fork()` to create multiple child processes, distributes integers from an input file, and collects primality-checking results through bidirectional parent-child communication.

## Overview

The program reads a set of integers from a text file and creates a separate child process to determine the primality of each integer.

Rather than allowing child processes to access the input array directly, the parent distributes integers through dedicated pipes. Each child independently performs its primality test and returns the result to the parent.

The parent uses `wait()` to collect completed processes and associates the results with their corresponding inputs using process IDs (PIDs).

**Key concepts demonstrated:**

- Process creation and management using `fork()`
- Inter-process communication using Unix pipes
- Bidirectional parent-child communication
- File descriptor management
- Low-level file operations using `open()`, `read()`, and `write()`
- Process synchronisation using `wait()`
- Dynamic memory allocation and resource cleanup
- Integer parsing and primality testing

## Architecture

The program follows a parent-worker architecture.

### Parent Process

The parent process is responsible for:

1. Opening and reading the input file.
2. Parsing the integers and allocating the required resources.
3. Creating two pipes for each child process.
4. Creating child processes using `fork()`.
5. Sending an integer to each child through its dedicated pipe.
6. Waiting for child processes to terminate.
7. Retrieving and displaying the primality results.
8. Closing file descriptors and releasing allocated memory.

### Child Processes

Each child process:

1. Closes unused pipe descriptors inherited from the parent.
2. Receives an integer through its parent-to-child pipe.
3. Determines whether the integer is prime.
4. Sends the result through its child-to-parent pipe.
5. Closes its remaining descriptors and terminates.

Two pipes are used per child to support communication in both directions.

## Primality Testing

The primality-checking function uses trial division to determine whether an integer is prime.

The implementation handles small values and even numbers directly, then tests odd divisors up to the square root of the input.

A short delay using `nanosleep()` is included during the checking loop, making differences in child-process completion times observable.

Results are displayed in **completion order**, rather than necessarily following the original input order.

## Repository Structure

```text
multi-process-prime-checker-c/
├── README.md
├── .gitignore
├── Makefile
├── prime.c
├── primeFunction.c
├── primeFunction.h
└── numbers.txt
```

### File Descriptions

| File | Description |
|---|---|
| `prime.c` | Main program handling file input, process creation, pipes, and result collection |
| `primeFunction.c` | Helper functions for integer conversion, file reading, primality checking, and output |
| `primeFunction.h` | Function declarations and shared constants |
| `Makefile` | Build configuration for compiling and linking the program |
| `numbers.txt` | Sample input containing integers for primality checking |

## Compilation and Execution

### Requirements

- GCC
- GNU Make or compatible `make`
- A POSIX-compatible environment, such as Linux or macOS

No external third-party libraries are required.

### Build

Compile the program using the provided Makefile:

```bash
make
```

This generates the executable:

```text
prime
```

### Run

Execute the program with the input file:

```bash
./prime numbers.txt
```

### Input Format

The input file contains one integer per line.

The **first line specifies the number of integers** to be processed.

For example:

```text
5
7
20
11
19
104729
```

This example creates five child processes, each responsible for checking one integer.

### Output

The program prints the child-process number, the integer processed, and its primality result.

Because the parent collects results as child processes finish, the output order may vary between executions.

### Clean

Remove the compiled executable and object files:

```bash
make clean
```

## Implementation Details

### Inter-Process Communication

Each child is assigned a dedicated pair of pipes:

- **Parent-to-child pipe:** transfers the integer to be checked.
- **Child-to-parent pipe:** returns the primality-checking result.

Unused file descriptors are closed to prevent unnecessary open pipe endpoints and manage system resources.

### Process Synchronisation

The parent tracks child PIDs in an array.

Using `wait()`, it retrieves the PID of a terminated child, identifies the corresponding array index, and reads the result from that child's pipe.

This allows results to be associated with the correct input even when processes complete in a different order from their creation.

### Low-Level File I/O

The project uses POSIX system calls rather than standard buffered file functions.

Integers are read from the input file character by character and converted into numeric values using custom helper functions.

## Limitations

- The program creates one child process per input integer, which may not scale efficiently to very large input sets.
- The input parser is designed for non-negative integer values in the expected file format.
- The implementation relies on POSIX process and pipe APIs and is not directly portable to native Windows environments.
- The trial-division algorithm is intended to demonstrate process communication rather than provide a highly optimised primality-testing solution.

## Project Background

This project was originally developed as a Unix systems programming exercise and has been organised for portfolio presentation.

The implementation focuses on understanding process management, operating-system interfaces, communication between independent processes, and resource management in C.

## Copyright

Copyright © 2026 Jian Heng Koo. All rights reserved.

The original source code, documentation, and other original materials in this repository are the work of Jian Heng Koo.

No licence is granted for copying, modifying, redistributing, or using these materials without permission from the author.

Third-party tools and referenced materials remain subject to their respective licences and copyright terms.
