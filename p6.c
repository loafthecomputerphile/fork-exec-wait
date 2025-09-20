#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        printf("Child running...\n");
        _exit(0);
    } else {
        int status;
        waitpid(pid, &status, 0); // explicitly wait for that child
        printf("Parent: waited for child %d\n", pid);
    }

    return 0;
}
