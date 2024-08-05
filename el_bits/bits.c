#include <el_bits.h>


int main() {
    unsigned int positiveNum;
    int negativeNum;
    unsigned char byte;

    printf("Введите положительное число: ");
    scanf("%u", &positiveNum);
    printf("Двоичное представление положительного числа: ");
    printBinary(positiveNum);
    printf("\n");

    printf("Введите отрицательное число: ");
    scanf("%d", &negativeNum);
    printf("Двоичное представление отрицательного числа: ");
    printNegativeBinary(negativeNum);
    printf("\n");

    printf("Введите положительное число для подсчета единиц: ");
    scanf("%u", &positiveNum);
    printf("Количество единиц в двоичном представлении: %d\n", countOnes(positiveNum));

    printf("Введите новое значение для третьего байта: ");
    scanf("%hhu", &byte);
    printf("Введите число для замены: ");
    scanf("%u", &positiveNum);
    printf("Новое число после замены: %u\n", replaceByte(positiveNum, byte));

    return 0;
}