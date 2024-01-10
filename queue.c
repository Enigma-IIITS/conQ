
#include <stdio.h>
#include <stdlib.h>

#define SIZE 20

typedef struct {
    int data[SIZE];
    int size;
    int front;
    int rear;
} SQUEUE;

void DEF(SQUEUE *queue) {
    queue->size = 0;
    queue->front = 0;
    queue->rear = -1;
}

void enqueue(SQUEUE *queue, int value) {
    if (queue->size >= SIZE) {
        fprintf(stderr, "Queue is full\n");
        return;
    }

    queue->rear = (queue->rear + 1) % SIZE;
    queue->data[queue->rear] = value;
    queue->size++;
}

int dequeue(SQUEUE *queue) {
    int value;

    if (queue->size <= 0) {
        fprintf(stderr, "Queue is empty\n");
        return -1; 
    }

    value = queue->data[queue->front];
    queue->front = (queue->front + 1) % SIZE;
    queue->size--;

    return value;
}
