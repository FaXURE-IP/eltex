#include <el_FM.h>

void change_directory(Panel *panel, const char *path)
{
    if (chdir(path) == 0)
    {
        getcwd(panel->path, sizeof(panel->path));
        list_files(panel->path, panel->files, &panel->file_count);
        panel->highlight = 0;
    }
    else
    {
        perror("chdir");
    }
}