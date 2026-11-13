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
