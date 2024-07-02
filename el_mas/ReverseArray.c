#include <el_mas.h>

void reverseArray(int *arr, int N) {
    int i, temp;
    for (i = 0; i < N / 2; i++) {
        temp = arr[i];
        arr[i] = arr[N - i - 1];
        arr[N - i - 1] = temp;
    }
}