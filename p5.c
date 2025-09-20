#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        printf("Child running...\n");
    } else {
        int status;
        pid_t cpid = wait(&status);
        printf("Parent: child %d finished with status %d\n", cpid, WEXITSTATUS(status));
    }

    return 0;
}
