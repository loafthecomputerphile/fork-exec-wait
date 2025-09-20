#include <stdio.h>
#include <unistd.h>

int main() {
    int x = 100;
    printf("Before fork: x = %d\n", x);

    pid_t pid = fork();

    if (pid == 0) { // child
        x += 50;
        printf("Child: x = %d\n", x);
    } else { // parent
        x -= 50;
        printf("Parent: x = %d\n", x);
    }

    return 0;
}
