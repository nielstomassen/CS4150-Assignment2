# CS4150-Assignment2

## How to run part 1

```bash
# 1. Compile both test1.cpp and test2.cpp
g++ test1.cpp -o test1
g++ test2.cpp -o test2

# 2. Compile sandbox.cpp into shared library
g++ -shared -fPIC -o sandbox.so sandbox.cpp

# 3. Run test1 and test2 while having the library loaded
LD_PRELOAD=./sandbox.so ./test1
LD_PRELOAD=./sandbox.so ./test2
```
## Part 2

### Introduction

This part implements a code scanner designed to identify syscall instructions (opcode: '0x0F 0x05') within the executable regions of a program. The scanner is implemented as a shared library that is loaded with 'LD_PRELOAD'.

### File topology
- pmparser.h: Header file from the github repository that the assignemt gives.
- pmparser.c: The programm from the github repository that the assignemt gives.
- codescanner.c: The code scanner that the assignment ask for implementation
- codescanner.so: The shared library

### Setup instruction
- **Building the Code:**
```bash
gcc -fPIC -shared -o codescanner.so codescanner.c pmparser.c
```
- **Running the Scanner:**
```bash
LD_PRELOAD=./codescanner.so /bin/ls
```

### Reults
* **Output**: When executed, the scanner will:
     * Parse /proc/self/maps to identify executable memory regions.
     * Scan these regions for syscall instructions and print their addresses.

Example Output:
```
Initialize Scanning...
Scanning for syscall instructions...
Scanning executable region: 0x7ffd5bbcc000-0x7ffd5bbce000
Syscall found at address: 0x7ffd5bbcc96b in region: 0x7ffd5bbcc000-0x7ffd5bbce000
Syscall found at address: 0x7ffd5bbccbb0 in region: 0x7ffd5bbcc000-0x7ffd5bbce000
Scanning Completed
```