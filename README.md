# Assignment: Practicing the Process API

Practicing with fork, exec, wait. 

### Overview

In this assignment, you will practice using the Process API to create processes and run programs under Linux. The goal is to gain hands-on experience with system calls related to process management. Specifically, you will practice using the unix process API functions 'fork()', 'exec()', 'wait()', and 'exit()'. 

⚠️ Note: This is not an OS/161 assignment. You will complete it directly on Linux. 

Use the Linux in your CSE4001 container. If you are using macOS, you may use the Terminal (you may need to install development tools with C/C++ compilers). 

**Reference Reading**: Arpaci-Dusseau, *Operating Systems: Three Easy Pieces*, Chapter 5 (Process API Basics)
 👉 [Chapter 5 PDF](http://pages.cs.wisc.edu/~remzi/OSTEP/cpu-api.pdf)

---

### **Steps to Complete the Assignment**

1. **Accept the GitHub Classroom Invitation**
    [GitHub Link](https://classroom.github.com/a/FZh4BrQG)

2. **Set up your Repository**
   
   - Clone the assignment repository.

3. **Study the Reference Materials**
   
   - Read **Chapter 5**.
   - Download and explore the sample programs from the textbook repository:
      [OSTEP CPU API Code](https://github.com/remzi-arpacidusseau/ostep-code/tree/master/cpu-api).

4. **Write Your Programs**
   
   - Adapt the provided example code to answer the assignment questions.
   - Each program should be clear, well-commented, and compile/run correctly.
   - Add your solution source code to the repository.

5. **Prepare Your Report**
   
   - Answer the questions in the README.md file. You must edit the README.md file and not create another file with the answers. 
   - For each question:
     - Include your **code**.
     - Provide your **answer/explanation**.

6. **Submit Your Work via GitHub**
   
   - Push both your **program code** to your assignment repository.
   - This push will serve as your submission.
   - Make sure all files, answers, and screenshots are uploaded and rendered properly.

---

### Questions

1. Write a program that calls `fork()`. Before calling `fork()`, have the main process access a variable (e.g., x) and set its value to something (e.g., 100). What value is the variable in the child process? What happens to the variable when both the child and parent change the value of x?

both process starts out with the value being 100. the child process gets its value changed to 150 while the parent is changed to 50 since they become copies of each other

```cpp
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

```

2. Write a program that opens a file (with the `open()` system call) and then calls `fork()` to create a new process. Can both the child and parent access the file descriptor returned by `open()`? What happens when they are writing to the file concurrently, i.e., at the same time?

both processes can access and write to the file but data may overlap

```cpp
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


```

3. Write another program using `fork()`.The child process should print “hello”; the parent process should print “goodbye”. You should try to ensure that the child process always prints first; can you do this without calling `wait()` in the parent?

if you call sleep you can ensue the child runs first

```cpp
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
```

4. Write a program that calls `fork()` and then calls some form of `exec()` to run the program `/bin/ls`. See if you can try all of the variants of `exec()`, including (on Linux) `execl()`, `execle()`, `execlp()`, `execv()`, `execvp()`, and `execvpe()`. Why do you think there are so many variants of the same basic call?

some can only take certain types of data like arrays (v) or lists (l), some can modify environment variables (e) and some also need  explicit paths (p)

```cpp
#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
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

```

5. Now write a program that uses `wait()` to wait for the child process to finish in the parent. What does `wait()` return? What happens if you use `wait()` in the child?

wait returns the process id of the process that finished and  if you use wait on a child it just returns -1 since children cant be parents.

```cpp
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

```

6. Write a slight modification of the previous program, this time using `waitpid()` instead of `wait()`. When would `waitpid()` be useful?

allows you to wait for a specific child. useful if you have multiple children where some need to run in the background and others are needed for the parent process to work

```cpp
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

```

7. Write a program that creates a child process, and then in the child closes standard output (`STDOUT FILENO`). What happens if the child calls `printf()` to print some output after closing the descriptor?

the child writes to closed file and is discarded while the parent's output is visible in the console

```cpp
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
```
