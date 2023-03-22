# CS415: Threads Lab
This program is used as practice to learn how to launch threads, protect shared data with semaphores, and implement important multithreaded design patterns.

<h3>Learning Goals</h3>
The primary learning goals of this project are:

* Learn how to launch threads.
* Learn how to protect shared data with semaphores.
* Learn how semaphores can be used to implement important multithreaded design patterns.

<h3>Program Description</h3>
The program consists of three parts: A, B, and C.

<h4>Part A</h4>
The program is written in C or C++ that launches a variable number of threads. Each thread is assigned one of the following jobs:

* Job one should print out "I am a producer," along with its TID (thread ID).
* Job two should print out "I am a consumer," along with its TID.

There should be an equal number of producers and consumers, which means it will always launch an even number of threads.

The program should be compiled with the following line:

```
[bash] gcc CS415-Thrd.c -lpthread
```
<h4>Part B</h4>
The threads assigned to be a producer should first add 1 to a global, shared variable A. Then they should add 3 to a second global, shared variable B. The threads assigned a consumer should first add 3 to the global variable B, and then add one to the global variable A. Each thread must sleep a random amount of time (from 0 microseconds to 100 microseconds) after each addition. They should do the addition 100 times each.

Two semaphores must be used, one to protect "A" from getting modified by two threads at the same time, and another to protect "B" from getting modified by two threads at the same time. The code should be written in a way that A and B can be modified at the same time by two different threads. In other words, one thread should never have access to modify A and B at the same time.

<h4>Part C</h4>
After the threads have completed part B, the threads assigned to be producers should "produce" a random number into a buffer. The threads assigned to be consumers should "consume" the number. Both threads should print the number they produce or consume along with their TID. Each thread should produce/consume 20 numbers. The buffer for the queue should only be able to hold three items at a time.
