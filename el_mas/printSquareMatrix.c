#include <el_mas.h>

void printSquareMatrix(int N) {
    int i, j, count = 1;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%d ", count++);
        }
        printf("\n");
    }
}