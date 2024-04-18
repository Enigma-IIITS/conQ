#include "queue.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int test_multi_threading() {
    // only for an example on how to create nd join the thread
    // pthread_mutex_t mutex;

    // pthread_mutex_init(&mutex, NULL);
    // pthread_t thread1;
    // pthread_t thread2;
    // int number = 2;

    // pthread_create(&thread1, NULL, &create_queue, NULL);
    // pthread_create(&thread2, NULL, &enqueue, (void*)&number);

    // pthread_join(thread1, NULL);
    // pthread_join(thread2, NULL);

    // pthread_mutex_destroy(&mutex);
    // return 0;

    /*
    TODO: create 2 arrays array of pthread contain MAX_THREADS no. of items
    spawn that many threads to enqueue and dequeue
    test whether queue is safe to interleaving of enqueue and dequeue operations
    */
}

void print_int(int data) {
    printf("%d\n", data);
}

int test_generic() {
    queue* q = create_queue(sizeof(int));
    enqueue(q, 4);
    enqueue(q, 8);
    printf("enqueue success!\n");
    int data = 0;
    dequeue(q, &data);
    printf("dequed: %d", data);
    display_queue(q, print_int);
}

int main() {

    printf("testing!\n");
    test_generic();
}