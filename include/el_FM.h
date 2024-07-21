#include <ncurses.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>

#define MAX_FILES 1024
#define MAX_FILENAME_LEN 256
#define PATH_MAX 1024

typedef struct
{
    char path[PATH_MAX];
    char files[MAX_FILES][MAX_FILENAME_LEN];
    int file_count;
    int highlight;
} Panel;

int compare_entries(const void *a, const void *b);
void display_panel(WINDOW *win, Panel *panel);
void change_directory(Panel *panel, const char *path);
void list_files(const char *path, char files[][MAX_FILENAME_LEN], int *file_count);