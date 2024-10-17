#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void print_process_info() {
    pid_t pid; 

    printf("Поки лише один процес\n");
    printf("Виклик функції fork...\n");
    
    pid = fork(); 

    if (pid < 0) {
        perror("Ошибка виклику fork, нащадка не створено");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        printf("Процес - нащадок, pid: %d, батьківський pid: %d\n", getpid(), getppid());
    } else {
        printf("Батьківський процес, pid: %d, pid нащадка: %d\n", getpid(), pid);
    }
}

int main(void) {
    print_process_info(); 
    return 0;
}
