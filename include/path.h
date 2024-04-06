#ifndef PATH_H
    #define PATH_H
    #define PATH_SIZE 500
    #define SNAPSHOT "_snapshot"
    #define SNAPSHOTS_FILE_TYPE ".csv"

    #include "memory_checks.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <ctype.h>

    typedef struct path_class{
        char *path;
    }Path_class;

    Path_class make_snap_dir_path(char *dir_path, char *cahce_dir);
    Path_class make_path(char *path);
    void delete_path(Path_class *to_del);
    void expand_path(Path_class * p, char * location_from_p);
#endif