#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

int calculate_points_in_circle(int num_throws) {
    int count_inside_circle = 0;
    // Ініціалізація генератора випадкових чисел з використанням часу та PID
    srand(time(NULL) ^ getpid());

    for (int i = 0; i < num_throws; i++) {
        // Генерація випадкових координат x і y
        double x = (double)rand() / RAND_MAX * 2.0 - 1.0; // x в діапазоні [-1, 1]
        double y = (double)rand() / RAND_MAX * 2.0 - 1.0; // y в діапазоні [-1, 1]

        // Перевірка, чи знаходиться точка всередині одиничного кола
        if (x * x + y * y <= 1.0) {
            count_inside_circle++;
        }
    }

    return count_inside_circle;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Використання: %s <кількість_кидків>\n", argv[0]);
        return 1; 
    }

    int num_throws = atoi(argv[1]);
    if (num_throws <= 0) {
        fprintf(stderr, "Кількість кидків повинна бути додатньою\n");
        return 1;
    }

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("Помилка створення процесу");
        return 1;
    }

    if (pid == 0) {
        // Дочірній процес: виконує обчислення методом Монте-Карло
        close(pipefd[0]); // Закриваємо читання
        int points_in_circle = calculate_points_in_circle(num_throws);
        write(pipefd[1], &points_in_circle, sizeof(points_in_circle)); // Передаємо результат через пайп
        close(pipefd[1]); // Закриваємо запис
        exit(0);
    } else {
        // Батьківський процес: очікує завершення дочірнього процесу
        close(pipefd[1]); // Закриваємо запис
        int points_in_circle;
        read(pipefd[0], &points_in_circle, sizeof(points_in_circle)); // Читаємо результат з пайпу
        close(pipefd[0]);

        wait(NULL); 
        double pi_estimate = (4.0 * points_in_circle) / num_throws;

        printf("Кількість точок всередині круга: %d\n", points_in_circle);
        printf("Оцінка числа π: %f\n", pi_estimate);
    }

    return 0;
}
