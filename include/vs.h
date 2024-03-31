#ifndef VERSION_CONTROLLER
    #define VERSION_CONTROLLER

    #define CACHE_DIR "FileSaverCache"
    #define CACHE_ENTRY_SIZE 500
    #define INDENT 2

    #include "fileoperations.h"
    #include "path.h"
    #include "cache_entry.h"

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <ctype.h>
    #include <stdbool.h>

    //system
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>

    void watch_directory(const char *dir_name);
    void save_snapshot(const char *dir_path);
#endif