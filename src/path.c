#include "path.h"



Path_class make_snap_dir_path(const char *dir_path, const char *cache_dir)
{
    char snap_dir_path[PATH_SIZE];
    strcpy(snap_dir_path, cache_dir);
    strcat(snap_dir_path, "/");
    strcat(snap_dir_path, dir_path);
    strcat(snap_dir_path, SNAPSHOT);
    strcat(snap_dir_path, SNAPSHOTS_FILE_TYPE);

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
    if(to_del.path != NULL)
        free(to_del.path);
}

void expand_path(Path_class * p, char * location_from_p)
{
    //+1 is from "/"
    char *temp = (char *)realloc(p->path, (strlen(p->path) + strlen(location_from_p) + 1) * sizeof(char));
    is_null(temp, REALOC_TEXT);

    p->path = temp;
    strcat(p->path, "/");
    strcat(p->path, location_from_p);
}


