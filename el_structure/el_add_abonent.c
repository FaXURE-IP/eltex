#include <el_structure.h>
#include <el_variable.h>

void add_abonent()
{
    abonent_count++;
    abonents = realloc(abonents, abonent_count * sizeof(abonent));
    if (abonents == NULL)
    {
        printf("Ошибка выделения памяти.\n");
        exit(1);
    }

    printf("Введите имя: ");
    scanf("%s", abonents[abonent_count - 1].name);
    printf("Введите фамилию: ");
    scanf("%s", abonents[abonent_count - 1].second_name);
    printf("Введите номер телефона: ");
    scanf("%s", abonents[abonent_count - 1].tel);

    printf("Абонент успешно добавлен.\n");
}
