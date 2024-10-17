#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

int main(void) {
    int status;

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork error");
        exit(1);
    }

    if (pid == 0) {

        printf("Дочірній процес (PID: %d) запущено. Виконуємо команду 'ls'.\n", getpid());
        execlp("ls", "ls", NULL);
        fprintf(stderr, "Exec error\n");
        exit(1);
    } else {

        printf("Батьківський процес (PID: %d) спить на 10 секунд.\n", getpid());
        printf("Перед викликом wait() (перевірка наявності зомбі процесу):\n");
        system("ps -l");

        sleep(10);

        printf("Після 10 секунд сну (перевірка наявності зомбі процесу):\n");
        system("ps -l");

        wait(&status);
        
        if (WIFEXITED(status)) {
            printf("Дочірній процес завершився з кодом: %d\n", WEXITSTATUS(status));
        }

        printf("Після виклику wait() (перевірка наявності зомбі процесу):\n");
        system("ps -l");
    }

    return 0;
}
