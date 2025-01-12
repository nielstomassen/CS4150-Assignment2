# CS4150-Assignment2

# How to run part 1
Compile both test1.cpp and test2.cpp using g++ test1.cpp -o test1
Compile sandbox.cpp into shared library using g++ -shared -fPIC -o sandbox.so sandbox.cpp
Run test1 and test2 while having the library loaded using: LD_PRELOAD=./sandbox.so ./test1
