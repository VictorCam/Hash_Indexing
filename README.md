# Hash_Indexing

Hash indexing using C++ with a csv file

## Compile Instruction:

--- 
```
1) g++ -std=c++11 main.cpp
2) ./a.out C
3) ./a.out L [employee id goes here]
```

## Description: 
---
- inital bucket/block size is 2
- max bucket/block size is 2
- split policy is 80%


## Assignment Details: 


Assume that we have a relation Employee(id, name, bio, manager-id). The values of id and manager-id are character strings with fixed size of 8 bytes. The values of name and bio are
1
character strings and take at most 200 and 500 bytes. Note that as opposed to the values of id
and manager-id, the sizes of the values of name (bio) are not fixed and are between 1 to 200 (500)
bytes. The size of each block is 4096 bytes (4KB). The size of each record is less than the size of a
block. Write a C++ program that reads an input Employee relation and builds a linear hash
index for the relation using attribute id. You may use a hash function of your choice. Your
program must increment the value of n if the average number of records per each block exceeds
80% of the block capacity. The input relation is stored in a CSV file, i.e., each tuple is in a
separate line and fields of each record are separated by commas. The program must also support
looking up a tuple using its id.
- The program must accept switch C for index creation mode and L for lookup in its
command line. The switch L is succeeded by the value of input id.
- Your program must assume that the input CSV file is in the current working directory, i.e.,
the one from which your program is running, and its name is Employee.csv
- The program must store the indexed relation in a file with the name EmployeeIndex on the
current working directory.