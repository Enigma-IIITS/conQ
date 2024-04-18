# conQ
Thread safe queue in C

//queue.c 

<stdio.h> is included for standard input/output functions
<stdlib.h> is included for dynamic memory allocation functions.

SIZE is used to define max size of queue to 20

The structure defines a Synchronized Queue containing a array data to store elements , size to store number of elements and two indexes front and rear .

initQueue void function helps to initialize a Synchronized Queue by setting its size to zero and front index to 0 and rear index to -1.

enqueue function is used to add elements to a Synchronized Queue. It first checks if the queue is full and if not increments the rear index , stores the value at rear index and increments the size.

dequeue function removes an element from the front of the queue , checks if the queue is empty and, if not, retrieves the value at the front, increments the front index  and decrements the size.

//test.c

#include "queue.c" includes the necessary headers of the synchronized queue. 

Producer function helps to enqueue integers. (taken for loops to add natural numbers ). 
Consumer function dequeue values / elements from the queue.

In the main function, a SynchronizedQueue named queue is declared and initialized.

The program simulates a basic producer-consumer scenario.
