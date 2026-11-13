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