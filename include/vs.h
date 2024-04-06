#ifndef VERSION_CONTROLLER
    #define VERSION_CONTROLLER

    #define CACHE_ENTRY_SIZE 500
    #define INDENT 4

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

    //pt open dir
    #include <dirent.h>

    enum dir_states{
        watched,
        unwatched,
        NOTdir,
        NOTpossible
    };

    enum dir_states is_dir_watched(char *dir_path, const char *CACHE_DIR);
    void save_snapshot(char *dir_path, const char *CACHE_DIR);

#endif