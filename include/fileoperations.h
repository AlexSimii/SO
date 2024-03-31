#ifndef FILE_OPERATIONS
    #define FILE_OPERATIONS

    #include "path.h"

    #include <stdio.h>
    #include<errno.h>
    #include<string.h>
    #include<ctype.h>
    #include <stdlib.h>
    #include <stdbool.h>

    //system
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>

    struct stat get_i_node(const Path_class path);
    bool check_file_is_dir(const Path_class path);
    bool check_dir_is_tracked(const char *dir_path, const char *cache_dir);

#endif