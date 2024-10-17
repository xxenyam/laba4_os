#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define DEFAULT_SECONDS 5 

int main(int argc, char *argv[]) {
    pid_t pid;
    time_t start_time, current_time;
    int count = 0;
    int seconds_to_run = DEFAULT_SECONDS;

    if (argc > 1) {
        seconds_to_run = atoi(argv[1]);
        if (seconds_to_run <= 0) {
            fprintf(stderr, "Введіть додатнє число секунд.\n");
            return 1;
        }
    }

    start_time = time(NULL);

    pid = fork(); 

    if (pid < 0) {
        perror("Ошибка виклику fork, нащадка не створено");
        exit(EXIT_FAILURE);
    }


    while (1) {
        current_time = time(NULL);
        

        if (difftime(current_time, start_time) >= seconds_to_run) {
            break; 
        }

        count++; 
    }

    if (pid == 0) {

        printf("Дочірній процес (pid: %d) виконав цикл %d разів.\n", getpid(), count);
    } else {

        printf("Батьківський процес (pid: %d) виконав цикл %d разів.\n", getpid(), count);
    }

    return 0;
}
