#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        // Example: execl
        execl("/bin/ls", "ls", "-l", NULL);

        // other ones
        // execlp("ls", "ls", "-l", NULL);
        // execv("/bin/ls", (char*[]){"ls", "-l", NULL});
        // execvp("ls", (char*[]){"ls", "-l", NULL});
        // execle("/bin/ls", "ls", "-l", NULL, environ);
        // execvpe("ls", (char*[]){"ls", "-l", NULL}, environ);

        perror("exec failed");
    } else {
        wait(NULL); // ensure parent waits
    }
    return 0;
}

