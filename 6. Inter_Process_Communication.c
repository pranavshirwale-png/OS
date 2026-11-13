
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <ctype.h>

#define INPUT_FIFO "/tmp/sentence_input_fifo"
#define OUTPUT_FIFO "/tmp/sentence_output_fifo"
#define RESULT_FILE "fifo_results.txt"
#define MAX_INPUT 10240

void remove_fifos()
{
    unlink(INPUT_FIFO);
    unlink(OUTPUT_FIFO);
}

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
        length -= written;
    }

    return 0;
}

void child_process()
{
    int input_fd = open(INPUT_FIFO, O_RDONLY);

    if (input_fd < 0)
    {
        perror("Input FIFO");
        _exit(1);
    }

    char input[MAX_INPUT + 1];
    size_t used = 0;
    char chunk[256];
    ssize_t count;

    while ((count = read(input_fd, chunk, sizeof(chunk))) > 0)
    {
        if ((size_t)count > MAX_INPUT - used)
        {
            fprintf(stderr, "Input too large!\n");
            close(input_fd);
            _exit(1);
        }

        memcpy(input + used, chunk, (size_t)count);
        used += (size_t)count;
    }

    close(input_fd);

    if (count < 0)
    {
        perror("Read FIFO");
        _exit(1);
    }

    input[used] = '\0';

    int characters = 0;
    int words = 0;
    int lines = 0;
    int in_word = 0;

    for (size_t i = 0; i < used; i++)
    {
        unsigned char ch = (unsigned char)input[i];

        if (ch != '\n')
            characters++;

        if (ch == '\n')
            lines++;

        if (isspace(ch))
        {
            in_word = 0;
        }
        else if (!in_word)
        {
            words++;
            in_word = 1;
        }
    }

    if (used > 0 && input[used - 1] != '\n')
        lines++;

    FILE *file = fopen(RESULT_FILE, "w");

    if (file == NULL)
    {
        perror("Result file");
        _exit(1);
    }

    fprintf(file, "Received sentences:\n%s", input);

    if (used > 0 && input[used - 1] != '\n')
        fputc('\n', file);

    fprintf(file,
            "\nCharacter count: %d\nWord count: %d\nLine count: %d\n",
            characters, words, lines);

    fclose(file);

    int output_fd = open(OUTPUT_FIFO, O_WRONLY);

    if (output_fd < 0)
    {
        perror("Output FIFO");
        _exit(1);
    }

    file = fopen(RESULT_FILE, "r");

    if (file == NULL)
    {
        perror("Read result file");
        close(output_fd);
        _exit(1);
    }

    char result[256];
    size_t result_count;

    while ((result_count = fread(result, 1, sizeof(result), file)) > 0)
    {
        if (write_all(output_fd, result, result_count) < 0)
        {
            perror("Write FIFO");
            fclose(file);
            close(output_fd);
            _exit(1);
        }
    }

    fclose(file);
    close(output_fd);

    _exit(0);
}

int main()
{
    remove_fifos();

    if (mkfifo(INPUT_FIFO, 0666) < 0)
    {
        perror("mkfifo input");
        return 1;
    }

    if (mkfifo(OUTPUT_FIFO, 0666) < 0)
    {
        perror("mkfifo output");
        remove_fifos();
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        remove_fifos();
        return 1;
    }

    if (pid == 0)
        child_process();

    printf("Enter sentences (finish with a blank line):\n");

    int input_fd = open(INPUT_FIFO, O_WRONLY);

    if (input_fd < 0)
    {
        perror("Open input FIFO");
        waitpid(pid, NULL, 0);
        remove_fifos();
        return 1;
    }

    char line[1024];

    while (fgets(line, sizeof(line), stdin) != NULL)
    {
        if (strcmp(line, "\n") == 0)
            break;

        if (write_all(input_fd, line, strlen(line)) < 0)
        {
            perror("Write input FIFO");
            close(input_fd);
            waitpid(pid, NULL, 0);
            remove_fifos();
            return 1;
        }
    }

    close(input_fd);

    int output_fd = open(OUTPUT_FIFO, O_RDONLY);

    if (output_fd < 0)
    {
        perror("Open output FIFO");
        waitpid(pid, NULL, 0);
        remove_fifos();
        return 1;
    }

    printf("\nResults received from the second process:\n");

    char result[256];
    ssize_t count;

    while ((count = read(output_fd, result, sizeof(result))) > 0)
    {
        write(STDOUT_FILENO, result, (size_t)count);
    }

    close(output_fd);

    waitpid(pid, NULL, 0);
    remove_fifos();

    return 0;
}

/*
OUTPUT :
Enter sentences (finish with a blank line):
Operating systems manage computer resources.
FIFOs allow processes to communicate.
Results received from the second process:
Received sentences:
Operating systems manage computer resources.
FIFOs allow processes to communicate.
Character count: 81
Word count: 11
Line count: 2
*/

"Sir/Ma'am, this program demonstrates Inter-Process Communication using FIFO, also called a Named Pipe. 
    The parent process accepts sentences from the user and sends them to the child process through an input FIFO. 
    The child process counts the characters, words, and lines, stores the result in a file, and sends the result back to the parent through another FIFO. 
    Finally, the parent displays the result."

    Q1. What is IPC?

IPC stands for Inter-Process Communication. It allows processes to exchange information.

Q2. What is FIFO?

FIFO stands for First In First Out. It is a named pipe used for communication between processes.

Q3. Why are two FIFOs used?

One FIFO sends input from parent to child, and the second sends results from child to parent.

Q4. What is the difference between pipe and FIFO?
- Pipe is generally used between related processes and has no filesystem name.
- FIFO has a filesystem name and can be used by unrelated processes.
Q5. What is mkfifo()?

It creates a named pipe.

Q6. What is unlink()?

It removes a file or named pipe from the filesystem.

Q7. Why is fork() used?

To create a child process for performing the calculations independently.

Q8. What is waitpid() used for?

It makes the parent wait for the child process to finish.

Q9. What is the use of isspace()?

It checks whether a character is whitespace, such as a space, tab, or newline.

Q10. What is the use of write_all()?

It ensures that all the given data is written successfully, handling partial writes.
