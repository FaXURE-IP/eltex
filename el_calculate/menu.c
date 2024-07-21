#include <menu.h>

void menu()
{
    printf("Выберите тип операции 1(Сумма) 2(Разность) 3(Произведение) 4(Частное)\n");

    int choose;
    if (scanf("%d", &choose) != 1)
    {
        printf("Ошибка ввода. Пожалуйста, введите целое число.\n");
        return;
    }

    float a, b;
    printf("Введите 2 числа для вычисления:\n");
    if (scanf("%f %f", &a, &b) != 2)
    {
        printf("Ошибка ввода. Пожалуйста, введите два числа.\n");
        return;
    }

    float s = 0;
    switch (choose)
    {
    case 1:
        s = summation(a, b);
        printf("Ответ: %f\n", s);
        break;
    case 2:
        s = subtruct(a, b);
        printf("Ответ: %f\n", s);
        break;
    case 3:
        s = multiplies(a, b);
        printf("Ответ: %f\n", s);
        break;
    case 4:
        s = division(a, b);
        printf("Ответ: %f\n", s);
        break;
    default:
        printf("Некорректный выбор операции.\n");
        break;
    }
}