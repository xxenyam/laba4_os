#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Використання: %s <кількість_кидків>\n", argv[0]);
        return 1; // Повертаємо код помилки
    }

    int num_throws = atoi(argv[1]); // Кількість "кидків"
    pid_t pid = fork();

    if (pid < 0) {
        // Помилка при створенні процесу
        perror("Fork error");
        return 1;
    }

    if (pid == 0) {
        // Дочірній процес
        int count_inside_circle = 0;

        // Ініціалізація генератора випадкових чисел
        srand(time(NULL));

        // Генерація "кидків"
        for (int i = 0; i < num_throws; i++) {
            double x = (double)rand() / RAND_MAX; // Випадкова x від 0 до 1
            double y = (double)rand() / RAND_MAX; // Випадкова y від 0 до 1

            // Перевірка, чи точка всередині круга
            if ((x * x + y * y) <= 1.0) {
                count_inside_circle++;
            }
        }

        // Повертаємо кількість точок всередині круга (обмежуємо до 255 для коду повернення)
        return count_inside_circle > 255 ? 255 : count_inside_circle;
    } else {
        // Батьківський процес
        int status;
        wait(&status); // Чекаємо на завершення дочірнього процесу

        if (WIFEXITED(status)) {
            int count_inside_circle = WEXITSTATUS(status);
            double pi_estimate = (4.0 * count_inside_circle) / num_throws;

            printf("Кількість точок всередині круга: %d\n", count_inside_circle);
            printf("Оцінка числа π: %f\n", pi_estimate);
        } else {
            printf("Дочірній процес завершився з помилкою.\n");
        }
    }

    return 0;
}
