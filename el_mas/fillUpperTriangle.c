#include <el_mas.h>

void fillUpperTriangle(int **matrix, int size) {
    // Заполняем верхний треугольник матрицы
    for (int i = 0; i < size; i++) {
        for (int j = i; j < size; j++) {
            matrix[i][j] = 1;
        }
    }
    

    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            int temp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = temp;
        }
    }

    // Второй шаг: Зеркальное отражение относительно середины ряда
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size / 2; j++) {
            int temp = matrix[i][j];
            matrix[i][j] = matrix[i][size - 1 - j];
            matrix[i][size - 1 - j] = temp;
        }
    }


    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}
