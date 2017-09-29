# Cliente-Servidor
This repository contains some implementations in c ++ for the client-server matter

## Folder Spotify_Proyect
In the Spotify_Proyect folder you will find a simple implementation of a music server.

If you want more songs you can download them from the following link

https://drive.google.com/open?id=0B76n4SHPvgJUZUt1bERKcThzZkU

to compile the program, simply make command.
In addition you must create a folder called splits

## Folder matmult

development of a program to multiply matrices that becomes progressively concurrent, the first versions do sequentially, the last will do concurrently intelligent

### mult0

Multiplication of matrices sequentially.
```
to compile
g++ -std=c++11 file_name.cpp -o file_name.out
```
```
to execute
./file_name.out graph1.dimacs
```

### mult1

Multiplication of matrices throwing a number of threads equal to the number of elements in the matrix

#### mult2

Multiplication of matrices by throwing a number of threads equal to the number of rows or columns in the matrix

### mult3

Multiplication of matrices throwing a number of threads equal to the number of cores of the computer
