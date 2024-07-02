#include <el_mas.h>

void fillMatrixSpiral(int N) {
    int matrix[N][N];
    int num = 1;
    int direction = 0; // Направление движения: 0 - вправо, 1 - вниз, 2 - влево, 3 - вверх
    int rowStart = 0, rowEnd = N - 1;
    int colStart = 0, colEnd = N - 1;
    
    while (rowStart <= rowEnd && colStart <= colEnd) {
        if (direction == 0) {
            for (int j = colStart; j <= colEnd; j++) {
                matrix[rowStart][j] = num++;
            }
            rowStart++;
        } else if (direction == 1) {
            for (int i = rowStart; i <= rowEnd; i++) {
                matrix[i][colEnd] = num++;
            }
            colEnd--;
        } else if (direction == 2) {
            for (int j = colEnd; j >= colStart; j--) {
                matrix[rowEnd][j] = num++;
            }
            rowEnd--;
        } else if (direction == 3) {
            for (int i = rowEnd; i >= rowStart; i--) {
                matrix[i][colStart] = num++;
            }
            colStart++;
        }
        direction = (direction + 1) % 4; // Изменяем направление движения
    }
    
    // Вывод матрицы
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}
