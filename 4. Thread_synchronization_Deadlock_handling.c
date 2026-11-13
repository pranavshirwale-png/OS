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

Sir/Ma'am, this program demonstrates the Dining Philosophers Problem in Operating Systems. 
 There are five philosophers sitting around a circular table with five forks. 
 Each philosopher needs two forks to eat.
Threads represent philosophers, mutex locks represent forks, and a semaphore allows only four philosophers to compete for forks at a time. 
 This helps prevent deadlock.


 Q1. What is the Dining Philosophers Problem?

It is a synchronization problem where five philosophers share five forks and need two forks to eat.

Q2. Why are five threads created?

Because each philosopher is represented by one thread.

Q3. Why are mutex locks used?

To ensure that only one philosopher uses a particular fork at a time.

Q4. Why is a semaphore used?

To allow a maximum of four philosophers to compete for forks simultaneously.

Q5. What is deadlock?

A situation where processes wait indefinitely for resources.

Q6. How is deadlock prevented in this program?

By allowing only four philosophers into the fork-acquisition process at a time, breaking the circular-wait condition.

Q7. What is the use of % in this program?

It helps calculate fork positions in a circular arrangement.

Q8. What is the critical section?

The section where a philosopher locks forks and accesses shared resources.

Q9. What is the difference between semaphore and mutex?

A semaphore controls resource availability, while a mutex provides exclusive access to a resource.

Q10. How many meals does each philosopher eat?

Three meals.
