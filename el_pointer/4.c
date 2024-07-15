#include <stdio.h>
#include <string.h>

char *my_strstr(const char *haystack, const char *needle)
{
    if (needle[0] == '\0')
        return (char *)haystack;
    int i, j;
    for (i = 0; haystack[i] != '\0'; i++)
    {
        for (j = 0; needle[j] != '\0'; j++)
        {
            if (haystack[i + j] != needle[j])
            {
                break;
            }
        }
        if (needle[j] == '\0')
        {
            return (char *)&haystack[i];
        }
    }
    return NULL;
}

int main()
{
    char str[100], substr[100];
    char *ptr;

    printf("Введите строку: ");
    fgets(str, sizeof(str), stdin);
    str[strlen(str) - 1] = '\0';

    printf("Введите подстроку: ");
    fgets(substr, sizeof(substr), stdin);
    substr[strlen(substr) - 1] = '\0';

    ptr = my_strstr(str, substr);

    if (ptr != NULL)
    {
        printf("Подстрока найдена в позиции: %ld\n", (ptr - str - strlen(substr)));
    }
    else
    {
        printf("Подстрока не найдена\n");
    }

    return 0;
}
