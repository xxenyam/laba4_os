#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int my_system(const char *command) {
    pid_t pid;
    int status;

    if ((pid = fork()) < 0) {
        perror("fork error");
        return -1; 
    }

    if (pid == 0) { 
     
        execlp(command, command, (char *) NULL);   
        perror("exec error");
        exit(1); /
    }

  
    if (wait(&status) < 0) {
        perror("wait error");
        return -1; 
    }

    if (WIFEXITED(status)) {
        return WEXITSTATUS(status); 
    } else {
        return -1; 
    }
}

int main(void) {
    const char *command = "ls"; 
    int result = my_system(command); 

    if (result != -1) {
        printf("Команда завершилася з кодом: %d\n", result);
    } else {
        printf("Сталася помилка під час виконання команди.\n");
    }

    return 0;
}
