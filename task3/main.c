#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

void create_child_processes(int num_processes) {
    for (int i = 0; i < num_processes; i++) {
        pid_t pid = fork(); 

        if (pid < 0) {
            perror("Помилка fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) { 
            printf("Дочірній процес %d: PID = %d, старт\n", i + 1, getpid());
            while (1) { 
                sleep(1); 
            }
            exit(0); 
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Використання: %s <кількість процесів>\n", argv[0]);
        return 1;
    }

    int num_processes = atoi(argv[1]); 

  
    if (num_processes <= 0) {
        fprintf(stderr, "Помилка: введіть натуральне число.\n");
        return 1;
    }

    create_child_processes(num_processes); 


    sleep(2);


    printf("Список працюючих процесів:\n");
    system("ps"); 

 
    printf("Видалити всі дочірні процеси? (y/n): ");
    char choice;
    scanf(" %c", &choice);

    if (choice == 'y' || choice == 'Y') {
       
        for (int i = 0; i < num_processes; i++) {
           
            pid_t pid_to_kill = getpid() + 1 + i; 
            kill(pid_to_kill, SIGKILL); 
            printf("Дочірній процес з PID %d завершено.\n", pid_to_kill);
        }
    }

    return 0;
}
