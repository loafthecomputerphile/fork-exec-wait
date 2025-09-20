#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {
    int fd = open("test.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0) { 
	perror("open"); 
	exit(1); 
    }

    pid_t pid = fork();

    if (pid == 0) { // child
        write(fd, "Child writing\n", 14);
    } else { // parent
        write(fd, "Parent writing\n", 15);
    }

    close(fd);
    return 0;
}
