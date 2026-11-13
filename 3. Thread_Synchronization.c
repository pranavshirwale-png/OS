#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define BUFFER_SIZE 5
#define ITEMS_TO_PRODUCE 10
int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;
sem_t empty_slots;
sem_t full_slots;
pthread_mutex_t mutex;
void *producer(void *arg)
{
 (void)arg;
 for (int item = 1; item <= ITEMS_TO_PRODUCE; item++) {
 sem_wait(&empty_slots);
 pthread_mutex_lock(&mutex);
 buffer[in] = item;
 printf("Producer produced: %d\n", item);
 in = (in + 1) % BUFFER_SIZE;
 pthread_mutex_unlock(&mutex);
 sem_post(&full_slots);
 sleep(1);
 }
 return NULL;
}
void *consumer(void *arg)
{
 (void)arg;
 for (int i = 0; i < ITEMS_TO_PRODUCE; i++) {
 sem_wait(&full_slots);
 pthread_mutex_lock(&mutex);
 int item = buffer[out];
 printf("Consumer consumed: %d\n", item);
 out = (out + 1) % BUFFER_SIZE;
 pthread_mutex_unlock(&mutex);
 sem_post(&empty_slots);
 sleep(2);
 }
 return NULL;
}
int main(void)
{
 pthread_t producer_thread;
 pthread_t consumer_thread;
 if (sem_init(&empty_slots, 0, BUFFER_SIZE) != 0 ||
 sem_init(&full_slots, 0, 0) != 0 ||
 pthread_mutex_init(&mutex, NULL) != 0) {
 perror("Initialization failed");
 return 1;
 }
 if (pthread_create(&producer_thread, NULL, producer, NULL) != 0 ||
 pthread_create(&consumer_thread, NULL, consumer, NULL) != 0) {
 perror("Thread creation failed");
 return 1;
 }
 pthread_join(producer_thread, NULL);
 pthread_join(consumer_thread, NULL);
 sem_destroy(&empty_slots);
 sem_destroy(&full_slots);
 pthread_mutex_destroy(&mutex);
 return 0;
}
/*
OUTPUT :
Producer produced: 1
Consumer consumed: 1
Producer produced: 2
Producer produced: 3
Consumer consumed: 2
Producer produced: 4
Producer produced: 5
Consumer consumed: 3
Producer produced: 6
Producer produced: 7
Consumer consumed: 4
Producer produced: 8
Consumer consumed: 5
Producer produced: 9
Consumer consumed: 6
Producer produced: 10
Consumer consumed: 7
Consumer consumed: 8
Consumer consumed: 9
Consumer consumed: 10
*/


"Sir/Ma'am, this program demonstrates the Producer-Consumer problem in Operating Systems. 
 The producer generates 10 items and stores them in a shared buffer of size 5.
 The consumer takes items from the buffer. 
 Semaphores are used to manage empty and full buffer slots, while a mutex lock prevents both threads from accessing the buffer simultaneously.

 Q1. What is the Producer-Consumer problem?

It is a synchronization problem where one process produces data and another process consumes it using a shared buffer.

Q2. Why is buffer size 5?

Because the buffer can store a maximum of five items at a time.

Q3. Why are two semaphores used?
- empty_slots tracks empty positions.
- full_slots tracks filled positions.
Q4. What is a mutex?

A mutex is a locking mechanism that allows only one thread to access a critical section at a time.

Q5. What is a thread?

A thread is the smallest unit of execution within a process.

Q6. What happens when the buffer is full?

The producer waits until the consumer removes an item.

Q7. What happens when the buffer is empty?

The consumer waits until the producer inserts an item.

Q8. What is a race condition?

A situation where multiple threads access shared data simultaneously and produce unpredictable results.

Q9. Why do we use pthread_join()?

To make the main thread wait until the producer and consumer threads complete execution.

Q10. What is the difference between semaphore and mutex?

A semaphore manages resource availability using a counter, whereas a mutex provides mutual exclusion using a lock.
