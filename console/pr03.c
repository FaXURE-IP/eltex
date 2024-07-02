#include <el_structure.h>

int main() {
    int choice;

    do {
        print_menu();
        printf("Введите выбранный пункт: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_abonent();
                break;
            case 2:
                delete_abonent();
                break;
            case 3:
                search_by_name();
                break;
            case 4:
                print_all();
                break;
            case 5:
                printf("Выход из программы.\n");
                break;
            default:
                printf("Неверный выбор. Пожалуйста, выберите пункт от 1 до 5.\n");
        }
    } while (choice != 5);

    return 0;
}
