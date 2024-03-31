#include "path.h"



Path_class make_snap_dir_path(const char *dir_path, const char *cache_dir)
{
    char snap_dir_path[PATH_SIZE];
    strcpy(snap_dir_path, cache_dir);
    strcat(snap_dir_path, "/");
    strcat(snap_dir_path, dir_path);
    strcat(snap_dir_path, SNAPSHOT);

    Path_class temp;
    temp.path = (char *)malloc(strlen(snap_dir_path) * sizeof(char));
    is_null(temp.path, ALOC_TEXT);

    strcpy(temp.path, snap_dir_path);

    return temp;
}

Path_class make_path(const char *path)
{
    Path_class temp;
    temp.path = (char *)malloc(strlen(path) * sizeof(char));
    is_null(temp.path, ALOC_TEXT);

    strcpy(temp.path, path);

    return temp;
}

void delete_path(Path_class to_del)
{
    free(to_del.path);
}
