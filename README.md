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

