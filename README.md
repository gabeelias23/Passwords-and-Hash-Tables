# Hash Table and Password Login System

This project implements four hash table collision-handling strategies in C++:

- Linear Probing
- Quadratic Probing
- Double Hashing
- Separate Chaining

The program demonstrates how each strategy handles collisions when inserting usernames into a hash table. It also implements a username and password login system using separate chaining. The login system lets users create accounts, look up and remove users, and keep track of the load factor. It also automatically increases the table size when the load factor goes above 0.75. Passwords are stored as hashes using a salt and pepper. 

## How to Compile 

Compile the program using a C++ compiler: 

g++ -std=c++17 HashTable.cpp -o HashTable

## How to Run

Run the compiled program: 

On Windows:

HashTable.exe

On Mac/Linux:



On Mac/Linux:

./HashTable
