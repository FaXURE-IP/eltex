#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Использование: %s <число>\n", argv[0]);
        return 1;
    }

    int number = atoi(argv[1]);
    int square = number * number;

    printf("Квадрат числа %d равен %d\n", number, square);

    return 0;
}
