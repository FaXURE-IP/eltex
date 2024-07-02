#include <stdio.h>

union IntAndBytes {
    int number;
    unsigned char bytes[4];
};

int main() {
    // Ввод исходного числа и нового значения для третьего байта
    union IntAndBytes data;
    unsigned char new_byte_value;

    printf("Введите положительное целое число: ");
    scanf("%d", &data.number);

    printf("Введите новое значение для третьего байта (0-255): ");
    scanf("%hhu", &new_byte_value);

    // Изменяем значение третьего байта
    data.bytes[2] = new_byte_value;

    // Вывод измененного числа
    printf("Измененное число: %d\n", data.number);

    return 0;
}
