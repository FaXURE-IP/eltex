#include <el_structure.h>
#include <el_variable.h>

void search_by_name()
{
    char name_to_find[NAME_LENGTH];
    int found = 0;

    printf("Введите имя абонента для поиска: ");
    scanf("%s", name_to_find);

    printf("Результаты поиска:\n");
    for (int i = 0; i < abonent_count; i++)
    {
        if (strcmp(abonents[i].name, name_to_find) == 0)
        {
            printf("Имя: %s, Фамилия: %s, Телефон: %s\n",
                   abonents[i].name, abonents[i].second_name, abonents[i].tel);
            found = 1;
        }
    }

    if (!found)
    {
        printf("Абонент с именем %s не найден в справочнике.\n", name_to_find);
    }
}