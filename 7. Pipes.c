
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define PATH_SIZE 1024
#define BUFFER_SIZE 512

int write_all(int fd, const char *data, size_t length)
{
    while (length > 0)
    {
        ssize_t written = write(fd, data, length);

        if (written < 0)
        {
            if (errno == EINTR)
                continue;
            return -1;
        }

        if (written == 0)
            return -1;

        data += written;
        length -= (size_t)written;
    }

    return 0;
}

int main(void)
{
    int parent_to_child[2];
    int child_to_parent[2];

    if (pipe(parent_to_child) == -1 ||
        pipe(child_to_parent) == -1)
    {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        /* Child process */

        close(parent_to_child[1]);
        close(child_to_parent[0]);

        char pathname[PATH_SIZE];
        size_t used = 0;
        char ch;
        ssize_t count;

        while (used < PATH_SIZE - 1)
        {
            count = read(parent_to_child[0], &ch, 1);

            if (count <= 0 || ch == '\n')
                break;

            pathname[used++] = ch;
        }

        pathname[used] = '\0';
        close(parent_to_child[0]);

        FILE *file = fopen(pathname, "rb");

        if (file == NULL)
        {
            char error_message[PATH_SIZE + 64];

            int length = snprintf(error_message,
                sizeof(error_message),
                "Error opening file '%s': %s\n",
                pathname, strerror(errno));

            if (length > 0)
                write_all(child_to_parent[1],
                          error_message, (size_t)length);

            close(child_to_parent[1]);
            _exit(1);
        }

        char buffer[BUFFER_SIZE];
        size_t bytes_read;

        while ((bytes_read = fread(buffer, 1,
                    sizeof(buffer), file)) > 0)
        {
            if (write_all(child_to_parent[1],
                          buffer, bytes_read) == -1)
            {
                perror("Child write error");
                fclose(file);
                close(child_to_parent[1]);
                _exit(1);
            }
        }

        if (ferror(file))
            perror("File read error");

        fclose(file);
        close(child_to_parent[1]);

        _exit(0);
    }

    /* Parent process */

    close(parent_to_child[0]);
    close(child_to_parent[1]);

    char pathname[PATH_SIZE];

    printf("Enter file pathname: ");

    if (fgets(pathname, sizeof(pathname), stdin) == NULL)
    {
        fprintf(stderr, "No pathname entered.\n");
        close(parent_to_child[1]);
        close(child_to_parent[0]);
        waitpid(pid, NULL, 0);
        return 1;
    }

    if (write_all(parent_to_child[1],
                  pathname, strlen(pathname)) == -1)
    {
        perror("Parent write error");
        close(parent_to_child[1]);
        close(child_to_parent[0]);
        waitpid(pid, NULL, 0);
        return 1;
    }

    close(parent_to_child[1]);

    printf("\nFile contents:\n");

    char buffer[BUFFER_SIZE];

    while ((count = read(child_to_parent[0],
                         buffer, sizeof(buffer))) > 0)
    {
        if (write_all(STDOUT_FILENO,
                      buffer, (size_t)count) == -1)
        {
            perror("Display error");
            break;
        }
    }

    if (count < 0)
        perror("Pipe read error");

    close(child_to_parent[0]);

    waitpid(pid, NULL, 0);

    return 0;
}

/*
OUTPUT :
Enter file pathname: sample.txt
File contents:
Pipes allow related processes to communicate.
The parent sends a pathname to the child.
The child returns the file contents.
*/


"Sir/Ma'am, this program demonstrates IPC using unnamed pipes in C. 
    First, two pipes are created and a child process is generated using fork(). 
    The parent process accepts a file pathname and sends it to the child through the first pipe.

Q1. What is a pipe?

A pipe is an IPC mechanism used to transfer data between processes.

Q2. What is an unnamed pipe?

It is a temporary communication channel without a filesystem name.

Q3. Why are two pipes used?

One sends the filename from parent to child, and the other sends file contents from child to parent.

Q4. What is the difference between pipe and FIFO?
- Pipe: Usually used between related processes.
- FIFO: Has a name in the filesystem and can be used by unrelated processes.
Q5. What is the use of pipe()?

It creates a communication channel with two file descriptors: one for reading and one for writing.

Q6. What is the use of fork()?

It creates a child process.

Q7. Why do we use close()?

To close unused file descriptors and prevent unnecessary resource usage.

Q8. What is the use of fread()?

It reads data from a file.

Q9. What is STDOUT_FILENO?

It represents standard output, usually the terminal, and its value is 1.

Q10. Why is waitpid() used?

To make the parent wait for the child process to complete
    The child opens the file, reads its contents, and sends them back through the second pipe. Finally, the parent displays the file contents."
