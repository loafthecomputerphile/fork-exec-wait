#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        close(STDOUT_FILENO);  // close stdout
        printf("This will not be visible\n");
    } else {
        printf("Parent still prints\n");
    }

    return 0;
}
