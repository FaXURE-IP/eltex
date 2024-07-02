#include <stdio.h>
#include <string.h>

int main() {
    char str[100], substr[100];
    char *ptr;

    printf("Введите строку: ");
    fgets(str, sizeof(str), stdin);
    str[strlen(str) - 1] = '\0';

    printf("Введите подстроку: ");
    fgets(substr, sizeof(substr), stdin);
    substr[strlen(substr) - 1] = '\0';

    ptr = strstr(str, substr);

    if (ptr != NULL) {
        printf("Подстрока найдена в позиции: %ld\n", (ptr - str - 1));
    } else {
        printf("Подстрока не найдена\n");
    }

    return 0;
}
