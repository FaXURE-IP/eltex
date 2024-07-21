#include <el_FM.h>

int main()
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    int highlight_panel = 0;
    int choice;

    Panel panels[2];
    getcwd(panels[0].path, sizeof(panels[0].path));
    getcwd(panels[1].path, sizeof(panels[1].path));
    list_files(panels[0].path, panels[0].files, &panels[0].file_count);
    list_files(panels[1].path, panels[1].files, &panels[1].file_count);

    WINDOW *left_win = newwin(LINES, COLS / 2, 0, 0);
    WINDOW *right_win = newwin(LINES, COLS / 2, 0, COLS / 2);

    while (1)
    {
        display_panel(left_win, &panels[0]);
        display_panel(right_win, &panels[1]);
        choice = getch();

        Panel *current_panel = &panels[highlight_panel];

        switch (choice)
        {
        case KEY_UP:
            if (current_panel->highlight > 0)
            {
                current_panel->highlight--;
            }
            break;
        case KEY_DOWN:
            if (current_panel->highlight < current_panel->file_count - 1)
            {
                current_panel->highlight++;
            }
            break;
        case '\n':
        { // Enter key
            struct stat statbuf;
            char new_path[PATH_MAX];
            strcpy(new_path, current_panel->path);
            strcat(new_path, "/");
            strcat(new_path, current_panel->files[current_panel->highlight]);

            if (strcmp(current_panel->files[current_panel->highlight], "..") == 0)
            {
                change_directory(current_panel, "..");
            }
            else if (stat(new_path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
            {
                change_directory(current_panel, new_path);
            }
            break;
        }
        case '\t': // Tab key
            highlight_panel = (highlight_panel + 1) % 2;
            break;
        case 'q':
            endwin();
            return 0;
        }
    }

    endwin();
    return 0;
}
