#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) { // child
        printf("hello\n");
    } else { // parent
        sleep(1);  // wait for child to finish
        printf("goodbye\n");
    }

    return 0;
}
