#include <el_FM.h>

void list_files(const char *path, char files[][MAX_FILENAME_LEN], int *file_count)
{
    DIR *dir = opendir(path);
    struct dirent *entry;
    *file_count = 0;

    if (dir == NULL)
    {
        perror("opendir");
        return;
    }

    strcpy(files[(*file_count)++], "..");

    while ((entry = readdir(dir)) != NULL && *file_count < MAX_FILES)
    {
        if (strcmp(entry->d_name, ".") != 0)
        {
            strcpy(files[(*file_count)++], entry->d_name);
        }
    }

    closedir(dir);

    qsort(&files[1], *file_count - 1, MAX_FILENAME_LEN, compare_entries);
}