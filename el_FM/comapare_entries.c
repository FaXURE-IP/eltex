#include <el_FM.h>

int compare_entries(const void *a, const void *b)
{
    const char *entry_a = (const char *)a;
    const char *entry_b = (const char *)b;
    struct stat stat_a, stat_b;

    stat(entry_a, &stat_a);
    stat(entry_b, &stat_b);

    if (S_ISDIR(stat_a.st_mode) && !S_ISDIR(stat_b.st_mode))
    {
        return -1;
    }
    else if (!S_ISDIR(stat_a.st_mode) && S_ISDIR(stat_b.st_mode))
    {
        return 1;
    }
    else
    {
        return strcmp(entry_a, entry_b);
    }
}
