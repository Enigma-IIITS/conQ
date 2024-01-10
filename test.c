
#include <stdio.h>
#include "queue.c" 

void producer(SQUEUE *queue) {
    for (int i = 1; i < 21; ++i) {
        enqueue(queue, i);
        printf("Produced: %d\n", i);
    }
}

void consumer(SQUEUE *queue) {
    for (int i = 1; i < 21; ++i) {
        int value = dequeue(queue);
        if (value != -1) {
            printf("Consumed: %d\n", value);
        } else {
            printf("Error: Unable to dequeue.\n");
        }
    }
}

int main() {
    SQUEUE queue;
    DEF(&queue);

    producer(&queue);
    consumer(&queue);

    return 0;
}
