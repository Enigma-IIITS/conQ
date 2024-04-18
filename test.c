#include "queue.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // only for an example on how to create nd join the thread
    pthread_mutex_t mutex;

    pthread_mutex_init(&mutex, NULL);
    pthread_t thread1;
    pthread_t thread2;
    int number = 2;

    pthread_create(&thread1, NULL, &createQueue, NULL);
    pthread_create(&thread2, NULL, &enqueue, (void*)&number);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_mutex_destroy(&mutex);
    return 0;
}