#include <el_structure.h>
#include <el_variable.h>

void delete_abonent()
{
    char name_to_delete[NAME_LENGTH];
    int found = 0;

    printf("Введите имя абонента для удаления: ");
    scanf("%s", name_to_delete);

    for (int i = 0; i < abonent_count; i++)
    {
        if (strcmp(abonents[i].name, name_to_delete) == 0)
        {
            for (int j = i; j < abonent_count - 1; j++)
            {
                abonents[j] = abonents[j + 1];
            }

            abonent_count--;
            abonents = realloc(abonents, abonent_count * sizeof(abonent));
            if (abonents == NULL && abonent_count > 0)
            {
                printf("Ошибка выделения памяти.\n");
                exit(1);
            }

            found = 1;
            printf("Абонент %s удален из справочника.\n", name_to_delete);
            break;
        }
    }

    if (!found)
    {
        printf("Абонент с именем %s не найден в справочнике.\n", name_to_delete);
    }
}