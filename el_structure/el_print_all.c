#include <el_structure.h>
#include <el_variable.h>

void print_all()
{
    if (abonent_count == 0)
    {
        printf("Справочник пуст.\n");
        return;
    }

    printf("Все записи в справочнике:\n");
    for (int i = 0; i < abonent_count; i++)
    {
        printf("Имя: %s, Фамилия: %s, Телефон: %s\n",
               abonents[i].name, abonents[i].second_name, abonents[i].tel);
    }
}