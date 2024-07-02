#include <el_mas.h>

int main() {
    int N;
    printf("Введите размер квадратной матрицы: ");
    scanf("%d", &N);
    printf("Квадратная матрица:\n");
    printSquareMatrix(N);

    printf("\nВведите размер массива: ");
    scanf("%d", &N);
    int arr[N], i;
    printf("Введите элементы массива: ");
    for (i = 0; i < N; i++) {
        scanf("%d", &arr[i]);
    }
    printf("Массив в обратном порядке: ");
    reverseArray(arr, N);
    printArray(arr, N);


     int **matrix = (int **)malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++) {
        matrix[i] = (int *)malloc(N * sizeof(int));
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = 0;
        }
    }
    printf("\nВывод верхнего треугольника");
    fillUpperTriangle(matrix, N);

    // Освобождаем выделенную память
    for (int i = 0; i < N; i++) {
        free(matrix[i]);
    }
    free(matrix);

    printf("\nЗаполнение матрицы числами от 1 до N спиралью:\n");
    fillMatrixSpiral(N);

    return 0;
}