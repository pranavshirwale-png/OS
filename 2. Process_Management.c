#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
static void sort(int a[], int n, int ascending)
{
 for (int i = 0; i < n - 1; i++) {
 for (int j = 0; j < n - i - 1; j++) {
 int swap = ascending ? a[j] > a[j + 1] : a[j] < a[j + 1];
 if (swap) {
 int temp = a[j];
 a[j] = a[j + 1];
 a[j + 1] = temp;
 }
 }
 }
}
static void print_array(const char *label, int a[], int n)
{
 printf("%s", label);
 for (int i = 0; i < n; i++)
 printf("%d ", a[i]);
 printf("\n");
}
int main(void)
{
 int n;
 printf("Enter number of integers: ");
 if (scanf("%d", &n) != 1 || n <= 0) {
 fprintf(stderr, "Invalid count.\n");
 return 1;
 }
 int *a = malloc((size_t)n * sizeof(*a));
 if (a == NULL) {
 perror("malloc");
 return 1;
 }
 printf("Enter %d integers: ", n);
 for (int i = 0; i < n; i++) {
 if (scanf("%d", &a[i]) != 1) {
 fprintf(stderr, "Invalid input.\n");
 free(a);
 return 1;
 }
 }
 pid_t child = fork();
 if (child < 0) {
 perror("fork");
 free(a);
 return 1;
 }
 if (child == 0) {
 sort(a, n, 0);
 printf("\nChild (PID %ld), descending: ", (long)getpid());
 print_array("", a, n);
 free(a);
 _exit(0);
 }
 sort(a, n, 1);
 printf("\nParent (PID %ld), ascending: ", (long)getpid());
 print_array("", a, n);
 int status;
 waitpid(child, &status, 0);
 printf("Parent waited for child PID %ld.\n", (long)child);
 free(a);
 /* Zombie demonstration: child exits; parent delays collecting its
status. */
 printf("\nZombie demonstration: check child PID with ps during the 10-second pause.\n");
 child = fork();
 if (child == 0)
 _exit(0);
 printf("Child PID %ld has exited; parent will wait 10 seconds before wait().\n",
 (long)child);
 fflush(stdout);
 sleep(10);
 waitpid(child, NULL, 0);
 printf("Parent collected the child's status; zombie is removed.\n");
 /* Orphan demonstration: parent exits while child continues. */
 printf("\nOrphan demonstration:\n");
 child = fork();
 if (child < 0) {
 perror("fork");
 return 1;
 }
 if (child == 0) {
 printf("Child PID %ld started; original parent PID was %ld.\n",
 (long)getpid(), (long)getppid());
 fflush(stdout);
 sleep(3);
 printf("Child is still running; new parent PID is %ld.\n",
 (long)getppid());
 return 0;
 }
 printf("Parent PID %ld exits; child PID %ld continues.\n",
 (long)getpid(), (long)child);
 return 0;
}

/*
OUTPUT :
Enter number of integers: 5
Enter 5 integers: 8 3 1 9 4
Parent (PID 4100), ascending: 1 3 4 8 9
Child (PID 4101), descending: 9 8 4 3 1
Parent waited for child PID 4101.
Zombie demonstration: check child PID with ps during the 10-second pause.
Child PID 4102 has exited; parent will wait 10 seconds before wait().
Parent collected the child's status; zombie is removed.
Orphan demonstration:
Parent PID 4100 exits; child PID 4103 continues.
Child PID 4103 started; original parent PID was 4100.
Child is still running; new parent PID is 1
*/



"Sir/Ma'am, this program demonstrates process management in Linux using C. It uses the fork() system call to create a child process. 
 The parent process sorts the numbers in ascending order, while the child process sorts them in descending order. 
 It also demonstrates zombie and orphan processes using process creation and termination

Q1. What is a process?

A process is a program that is currently executing.

Q2. What is fork()?

It is a system call used to create a child process.

Q3. What is the difference between parent and child process?

The parent creates the child using fork(). Both execute independently.

Q4. What is PID?

PID stands for Process ID. It uniquely identifies a process.

Q5. What is a zombie process?

A terminated child process whose exit status has not yet been collected by its parent.

Q6. What is an orphan process?

A child process whose parent has terminated while the child is still running.

Q7. How can we remove a zombie process?

The parent should collect its exit status using wait() or waitpid().

Q8. What is the difference between getpid() and getppid()?
- getpid() returns the current process ID.
- getppid() returns the parent process ID.
Q9. What is the use of waitpid()?

It makes the parent wait for a specific child process and collect its termination status.

Q10. Which sorting technique is used?

Bubble Sort. 

