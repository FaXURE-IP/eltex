#include <el_structure.h>
#include <el_variable.h>

void add_abonent()
{
    if (abonent_count >= MAX_ABONENTS)
    {
        printf("Справочник переполнен. Нельзя добавить больше абонентов.\n");
        return;
    }

    printf("Введите имя: ");
    scanf("%s", abonents[abonent_count].name);
    printf("Введите фамилию: ");
    scanf("%s", abonents[abonent_count].second_name);
    printf("Введите номер телефона: ");
    scanf("%s", abonents[abonent_count].tel);

    abonent_count++;
    printf("Абонент успешно добавлен.\n");
}
