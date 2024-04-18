#include "queue.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct node
{
  void *data;
  struct node *next;
} node;

node *createNode(void *data, size_t required_size)
{
  node *newnode = (node *)malloc(sizeof(node));
  if(newnode == NULL)
    return NULL;

  newnode->data = malloc(required_size);
  if(newnode->data == NULL)
  {
    free(newnode);
    return NULL;
  } //void *memcpy(void *to, const void *from, size_t numBytes);
  memcpy(newnode->data, data, required_size);
  newnode->next = NULL;

  return newnode;
}

typedef struct queue
{
  size_t size;
  size_t allocated_size;
  node *head;
  node *tail;
   pthread_mutex_t mutex ;
} queue;

queue *createQueue(size_t required_size)
{
  queue *q = (queue *)malloc(sizeof(queue));
  if(q == NULL)
    return NULL;

  q->allocated_size = required_size;
  q->size = 0;
  q->head = q->tail = NULL;
   
  return q;
}

queue *enqueue(queue *q, void *data)
{
  
   pthread_mutex_lock(q->mutex);
  if(q == NULL)
    return NULL;

  node *newnode = createNode(data, q->allocated_size);
  if(newnode == NULL)
  {
    return NULL;
  }

  if(q->size == 0)
  { 
    q->head = q->tail = newnode;
  }
  else
  {
    q->tail->next = newnode;
    q->tail = newnode;
  }

  q->size++;
 pthread_mutex_unlock(q->mutex);
  return q;
}

queue *dequeue(queue *q, void *data)
{ pthread_mutex_lock(q->mutex);
  if(q == NULL)
    return NULL;
  if(q->size == 0)
    return NULL;

  node *toDelete = q->head;
  if(q->size == 1)
  {
    memcpy(data, toDelete->data, q->allocated_size);
    free(toDelete->data);
    free(toDelete);
    q->head = q->tail = NULL;
    q->size--;
    return q;
  }

  q->head = q->head->next;
  memcpy(data, toDelete->data, q->allocated_size);
  free(toDelete->data);
  free(toDelete);
  q->size--;

pthread_mutex_unlock(q->mutex); 
  return q;
}

queue *front(queue *q, void *data)
{
  if(q == NULL)
    return NULL;

  if(q->size == 0)
    return NULL;

  memcpy(data, q->head->data, q->allocated_size);

  return q;
}

queue *reverse(queue *q)
{  pthread_mutex_lock(q->mutex);
  if(q == NULL)
    return NULL;
  if(q->size == 0)
    return q;
  else
  {
    void *data = malloc(q->allocated_size);
    if(data != NULL)
    {
      dequeue(q, data);
      reverse(q);
      enqueue(q, data);
      free(data);
    }
    return q;
  } pthread_mutex_unlock(q->mutex);
}

queue *clearQueue(queue *q)
{pthread_mutex_lock(q->mutex);
  if(q == NULL)
    return NULL;

  while(!isEmpty(q))
  {
    node *temp = q->head;
    q->head = q->head->next;
    free(temp->data);
    free(temp);
    q->size--;
  }
 pthread_mutex_unlock(q->mutex);
  return q;
}

size_t getSize(queue *q)
{ 
    pthread_mutex_lock(q->mutex);
  if(q == NULL)
  {
    return 0;
  }
pthread_mutex_unlock(q->mutex);
  return q->size;
}

bool isEmpty(queue *q)
{
  return q->size == 0 ? true : false;
}

size_t getallocated_size(queue *q)
{
  if(q == NULL)
  {
    return 0;
  }

  return q->allocated_size;
}

queue *copyQueue(queue *src)
{ 
    pthread_mutex_lock(src->mutex);
  if(src == NULL)
  {
    return NULL;
  }

  queue *newQueue = createQueue(src->allocated_size);
  if(newQueue == NULL)
  {
    return NULL;
  }

  // Iterate through original queue and copy nodes
  node *currentOriginalNode = src->head;
  node *previousNewNode = NULL;
  while(currentOriginalNode != NULL)
  {
    enqueue(newQueue, currentOriginalNode->data);
    currentOriginalNode = currentOriginalNode->next;
  }
pthread_mutex_unlock(src->mutex);
  return newQueue;
}


int main(){
  // only for an example on how to create nd join the thread
pthread_mutex_t mutex;

pthread_mutex_init(&mutex, NULL);
pthread_t thread1;
pthread_t thread2;
int number=2;

pthread_create(&thread1,NULL,&createQueue,NULL);
pthread_create(&thread2,NULL,&enqueue,(void*)&number);

pthread_join(thread1,NULL);
pthread_join(thread2,NULL);

pthread_mutex_destroy(&mutex);
return 0;
}