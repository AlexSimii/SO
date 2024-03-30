#include "vs.h"
#include "fileoperations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//system
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void watch_directory(const char *dir_path){
    printf("working on directory:%s\n", dir_path);
    struct stat i_node = get_i_node(dir_path);

    if(S_ISDIR(i_node.st_mode))
    {
        char snap_dir_path[500];
        strcpy(snap_dir_path, CACHE_DIR);
        strcat(snap_dir_path, "/");
        strcat(snap_dir_path, dir_path);
        strcat(snap_dir_path, SNAPSHOT);
        struct stat snap_i_node = check_file_exists(snap_dir_path);

        if(S_ISDIR(snap_i_node.st_mode))
            printf("Directory is tracked\n");
        else
            printf("Directory is untracked\n");

    }
    else{
        printf("An argument was given, but not a dir\n");
        exit(EXIT_FAILURE);
    }
}
