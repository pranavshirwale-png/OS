#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define PHILOSOPHERS 5
#define MEALS 3
pthread_mutex_t forks[PHILOSOPHERS];
sem_t room;
void *philosopher(void *arg)
{
 int id = *(int *)arg;
 int left_fork = id;
 int right_fork = (id + 1) % PHILOSOPHERS;
 for (int meal = 1; meal <= MEALS; meal++) 
 {
    printf("Philosopher %d is thinking.\n", id);
    sleep(1);
    sem_wait(&room); // Allow at most four philosophers to compete for forks
    pthread_mutex_lock(&forks[left_fork]);
    pthread_mutex_lock(&forks[right_fork]);
    printf("Philosopher %d is eating (meal %d).\n", id, meal);
    sleep(1);
    pthread_mutex_unlock(&forks[right_fork]);
    pthread_mutex_unlock(&forks[left_fork]);
    sem_post(&room);
 }
 printf("Philosopher %d has finished eating.\n", id);
 return NULL;
}
int main(void)
{
 pthread_t threads[PHILOSOPHERS];
 int ids[PHILOSOPHERS];
 if (sem_init(&room, 0, PHILOSOPHERS - 1) != 0) {
 perror("sem_init");
 return 1;
 }
 for (int i = 0; i < PHILOSOPHERS; i++) {
 if (pthread_mutex_init(&forks[i], NULL) != 0) {
 perror("pthread_mutex_init");
 return 1;
 }
 }
 for (int i = 0; i < PHILOSOPHERS; i++) {
 ids[i] = i;
 if (pthread_create(&threads[i], NULL, philosopher, &ids[i]) != 0)
{
 perror("pthread_create");
 return 1;
 }
 }
 for (int i = 0; i < PHILOSOPHERS; i++)
 pthread_join(threads[i], NULL);
 for (int i = 0; i < PHILOSOPHERS; i++)
 pthread_mutex_destroy(&forks[i]);
 sem_destroy(&room);
 printf("All philosophers finished without deadlock.\n");
 return 0;
}
/*
OUTPUT :
Philosopher 0 is thinking.
Philosopher 1 is thinking.
Philosopher 2 is thinking.
Philosopher 3 is thinking.
Philosopher 4 is thinking.
Philosopher 0 is eating (meal 1).
Philosopher 2 is eating (meal 1).
Philosopher 1 is thinking.
Philosopher 3 is thinking.
Philosopher 4 is thinking.
Philosopher 0 is thinking.
Philosopher 2 is thinking.
Philosopher 1 is eating (meal 1).
Philosopher 3 is eating (meal 1).
Philosopher 4 is eating (meal 1).
Philosopher 0 is eating (meal 2).
Philosopher 2 is eating (meal 2).
Philosopher 1 is thinking.
Philosopher 3 is thinking.
Philosopher 4 is thinking.
Philosopher 0 is thinking.
Philosopher 2 is thinking.
Philosopher 1 is eating (meal 2).
Philosopher 3 is eating (meal 2).
Philosopher 4 is thinking.
Philosopher 0 is eating (meal 3).
Philosopher 2 is thinking.
Philosopher 1 is thinking.
Philosopher 3 is thinking.
Philosopher 4 is eating (meal 2).
Philosopher 0 has finished eating.
Philosopher 2 is eating (meal 3).
Philosopher 1 is eating (meal 3).
Philosopher 3 is eating (meal 3).
Philosopher 4 is thinking.
Philosopher 2 has finished eating.
Philosopher 1 has finished eating.
Philosopher 3 has finished eating.
Philosopher 4 is eating (meal 3).
Philosopher 4 has finished eating.
All philosophers finished without deadlock.
*/