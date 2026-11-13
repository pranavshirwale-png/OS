
#include <stdio.h>
#include <stdlib.h>

#define MAX_REQUESTS 100

void sort(int a[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (a[j] > a[j + 1])
            {
                int temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
        }
    }
}

void print_result(const char *name, int order[],
                  int count, int movement)
{
    printf("\n%s service order: ", name);

    for (int i = 0; i < count; i++)
    {
        printf("%d", order[i]);

        if (i != count - 1)
            printf(" -> ");
    }

    printf("\nTotal head movement: %d cylinders\n", movement);
}

/* SSTF Algorithm */
void sstf(int requests[], int n, int head)
{
    int visited[MAX_REQUESTS] = {0};
    int order[MAX_REQUESTS];
    int movement = 0;
    int current = head;

    for (int done = 0; done < n; done++)
    {
        int nearest = -1;

        for (int i = 0; i < n; i++)
        {
            if (!visited[i] &&
                (nearest == -1 ||
                abs(requests[i] - current) <
                abs(requests[nearest] - current)))
            {
                nearest = i;
            }
        }

        visited[nearest] = 1;
        movement += abs(requests[nearest] - current);
        current = requests[nearest];
        order[done] = current;
    }

    print_result("SSTF", order, n, movement);
}

/* SCAN Algorithm */
void scan(int requests[], int n, int head, int max_cylinder)
{
    int sorted[MAX_REQUESTS];
    int order[MAX_REQUESTS];
    int count = 0;
    int movement = 0;
    int current = head;

    for (int i = 0; i < n; i++)
        sorted[i] = requests[i];

    sort(sorted, n);

    for (int i = 0; i < n; i++)
    {
        if (sorted[i] >= head)
        {
            order[count++] = sorted[i];
            movement += abs(sorted[i] - current);
            current = sorted[i];
        }
    }

    if (current != max_cylinder)
    {
        movement += max_cylinder - current;
        current = max_cylinder;
    }

    for (int i = n - 1; i >= 0; i--)
    {
        if (sorted[i] < head)
        {
            order[count++] = sorted[i];
            movement += abs(sorted[i] - current);
            current = sorted[i];
        }
    }

    print_result("SCAN", order, count, movement);
}

/* C-LOOK Algorithm */
void c_look(int requests[], int n, int head)
{
    int sorted[MAX_REQUESTS];
    int order[MAX_REQUESTS];
    int count = 0;
    int movement = 0;
    int current = head;

    for (int i = 0; i < n; i++)
        sorted[i] = requests[i];

    sort(sorted, n);

    for (int i = 0; i < n; i++)
    {
        if (sorted[i] >= head)
        {
            order[count++] = sorted[i];
            movement += abs(sorted[i] - current);
            current = sorted[i];
        }
    }

    int lowest = -1;

    for (int i = 0; i < n; i++)
    {
        if (sorted[i] < head)
            lowest = i;
    }

    if (lowest >= 0)
    {
        movement += abs(current - sorted[lowest]);
        current = sorted[lowest];
        order[count++] = current;

        for (int i = lowest + 1; i < n; i++)
        {
            order[count++] = sorted[i];
            movement += abs(sorted[i] - current);
            current = sorted[i];
        }
    }

    print_result("C-LOOK", order, count, movement);
}

int main()
{
    int n, head, max_cylinder;
    int requests[MAX_REQUESTS];

    printf("Enter number of disk requests (1-%d): ",
           MAX_REQUESTS);

    if (scanf("%d", &n) != 1 ||
        n < 1 || n > MAX_REQUESTS)
    {
        printf("Invalid number of requests.\n");
        return 1;
    }

    printf("Enter request queue: ");

    for (int i = 0; i < n; i++)
    {
        if (scanf("%d", &requests[i]) != 1)
        {
            printf("Invalid request.\n");
            return 1;
        }
    }

    printf("Enter initial head position: ");

    if (scanf("%d", &head) != 1)
    {
        printf("Invalid head position.\n");
        return 1;
    }

    printf("Enter maximum cylinder number: ");

    if (scanf("%d", &max_cylinder) != 1 ||
        max_cylinder <= 0 ||
        head < 0 || head > max_cylinder)
    {
        printf("Invalid cylinder range or head position.\n");
        return 1;
    }

    for (int i = 0; i < n; i++)
    {
        if (requests[i] < 0 ||
            requests[i] > max_cylinder)
        {
            printf("Request outside cylinder range.\n");
            return 1;
        }
    }

    printf("\nInitial head direction: toward higher-numbered cylinders\n");

    sstf(requests, n, head);
    scan(requests, n, head, max_cylinder);
    c_look(requests, n, head);

    return 0;
}

/*OUTPUT :
INPUT
Enter number of disk requests (1-100): 8
Enter request queue: 98 183 37 122 14 124 65 67
Enter initial head position: 53
Enter maximum cylinder number: 199
OUTPUT
Initial head direction: toward higher-numbered cylinders
SSTF service order: 65 -> 67 -> 37 -> 14 -> 98 -> 122 -> 124 -> 183
Total head movement: 236 cylinders
SCAN service order: 65 -> 67 -> 98 -> 122 -> 124 -> 183 -> 37 -> 14
Total head movement: 331 cylinders
C-LOOK service order: 65 -> 67 -> 98 -> 122 -> 124 -> 183 -> 14 -> 37
Total head movement: 322 cylinders
*/