#include <el_FM.h>

void display_panel(WINDOW *win, Panel *panel)
{
    int x, y;
    getmaxyx(win, y, x);
    wclear(win);

    mvwprintw(win, 0, 0, "%s", panel->path);

    for (int i = 0; i < panel->file_count; i++)
    {
        if (i == panel->highlight)
        {
            wattron(win, A_REVERSE);
        }
        mvwprintw(win, i + 1, 1, "%s", panel->files[i]);
        wattroff(win, A_REVERSE);
    }

    box(win, 0, 0);
    wrefresh(win);
}