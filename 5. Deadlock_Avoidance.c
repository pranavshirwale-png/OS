#include <stdio.h>
#define MAX_PROCESSES 10
#define MAX_RESOURCES 10
int main(void)
{
 int processes, resources;
 int allocation[MAX_PROCESSES][MAX_RESOURCES];
 int maximum[MAX_PROCESSES][MAX_RESOURCES];
 int available[MAX_RESOURCES];
 int need[MAX_PROCESSES][MAX_RESOURCES];
 int finished[MAX_PROCESSES] = {0};
 int safe_sequence[MAX_PROCESSES];
 printf("Enter number of processes: ");
 if (scanf("%d", &processes) != 1 ||
 processes <= 0 || processes > MAX_PROCESSES) {
 printf("Invalid number of processes.\n");
 return 1;
 }
 printf("Enter number of resource types: ");
 if (scanf("%d", &resources) != 1 ||
 resources <= 0 || resources > MAX_RESOURCES) {
 printf("Invalid number of resource types.\n");
 return 1;
 }
 printf("Enter Allocation Matrix (%d x %d):\n", processes, resources);
 for (int i = 0; i < processes; i++)
 for (int j = 0; j < resources; j++)
 scanf("%d", &allocation[i][j]);
 printf("Enter Maximum Demand Matrix (%d x %d):\n", processes,
resources);
 for (int i = 0; i < processes; i++)
 for (int j = 0; j < resources; j++)
 scanf("%d", &maximum[i][j]);
 printf("Enter Available Resources (%d values):\n", resources);
 for (int j = 0; j < resources; j++)
 scanf("%d", &available[j]);
 /* Calculate Need = Maximum - Allocation. */
 for (int i = 0; i < processes; i++) {
 for (int j = 0; j < resources; j++) {
 if (maximum[i][j] < allocation[i][j]) {
 printf("Invalid input: allocation exceeds maximum demand.\n");
 return 1;
 }
 need[i][j] = maximum[i][j] - allocation[i][j];
 }
 }
 int work[MAX_RESOURCES];
 for (int j = 0; j < resources; j++)
 work[j] = available[j];
 int completed = 0;
 while (completed < processes) {
 int found = 0;
 for (int i = 0; i < processes; i++) {
 if (finished[i])
 continue;
 int can_finish = 1;
 for (int j = 0; j < resources; j++) {
 if (need[i][j] > work[j]) {
 can_finish = 0;
break;
 }
 }
 if (can_finish) {
 for (int j = 0; j < resources; j++)
 work[j] += allocation[i][j];
 safe_sequence[completed++] = i;
 finished[i] = 1;
 found = 1;
 }
 }
 if (!found)
 break;
 }
 if (completed == processes) {
 printf("\nSystem is in a SAFE state.\nSafe sequence: ");
 for (int i = 0; i < processes; i++)
 printf("P%d%s", safe_sequence[i],
 (i == processes - 1) ? "\n" : " -> ");
 } else {
 printf("\nSystem is in an UNSAFE state. No safe sequence exists.\n");
 }
 return 0;
}
/*
OUTPUT :
Enter number of processes: 5
Enter number of resource types: 3
Enter Allocation Matrix (5 x 3):
0 1 0
2 0 0
3 0 2
2 1 1
0 0 2
Enter Maximum Demand Matrix (5 x 3):
7 5 3
3 2 2
9 0 2
2 2 2
4 3 3
Enter Available Resources (3 values):
3 3 2
*/


Sir/Ma'am, this program implements Banker's Algorithm, which is used for deadlock avoidance in Operating Systems.
 It takes the Allocation Matrix, Maximum Demand Matrix, and Available Resources as input. 
 It calculates the Need Matrix and checks whether all processes can complete execution safely.

 Q1. What is Banker's Algorithm?

It is a deadlock avoidance algorithm used to check whether resource allocation is safe.

Q2. Why is it called Banker's Algorithm?

Because it works like a banker who grants loans only when sufficient resources remain to satisfy future demands safely.

Q3. What is a safe sequence?

An order in which all processes can complete their execution.

Q4. What is the Need Matrix formula?

Need = Maximum − Allocation.

Q5. What is the use of the Available array?

It stores the number of currently available resources of each type.

Q6. What is the purpose of the Work array?

It temporarily tracks available resources while checking processes.

Q7. What is the purpose of finished[]?

It tracks which processes have completed execution.

Q8. What happens when no process can execute?

The system is declared unsafe because no complete safe sequence is found.

Q9. What is deadlock avoidance?

It is a technique that prevents the system from entering an unsafe state through careful resource allocation.

Q10. What is the difference between safe and unsafe states?
- Safe: A safe sequence exists.
- Unsafe: No safe sequence exists.
 If all processes can finish, the system is in a safe state; otherwise, it is in an unsafe state.
