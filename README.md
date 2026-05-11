# Set-Operations-Software

This project implements an interactive "pocket calculator" system in C, enabling users to perform various mathematical operations on sets of integers. 

## Overview
The system manages 6 predefined sets (SETA through SETF) capable of storing integers in the range [0..127]. It reads commands from the standard input (interactive or file redirection), parses the commands, validates the format, and invokes the appropriate set operations using highly efficient bitwise arithmetic.

## Features
* **Memory Efficient:** Sets are implemented using arrays of bits (128 bits total per set). Each bit corresponds to an integer, ensuring minimal memory usage.
* **Set Operations:** Perform union, intersection, subtraction, and symmetric difference on sets.
* **Error Handling:** Comprehensive input validation detecting missing commas, extraneous text, out-of-range values, and undefined commands.
* **Dynamic Input:** Reads multiple commands sequentially until termination.

## Installation
To compile the project, ensure you have `gcc` and `make` installed. Use the provided `makefile` to compile the project cleanly (compiled with `-ansi -pedantic -Wall` flags):



* **Author:** Naomi Aharoni
* ** January 2026
* **Final Grade:** 100%
```bash
make
