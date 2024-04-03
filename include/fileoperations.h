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

    //pt open
    #include <fcntl.h>
    //pt open dir
    #include <dirent.h>

    struct stat get_i_node(const Path_class path);
    bool is_dir(const Path_class path);
    bool is_file(const Path_class path);
    bool check_dir_is_tracked(const char *dir_path, const char *cache_dir);
    int open_snapshot_file_for_cache(Path_class p);
    DIR *open_director(Path_class p);

#endif