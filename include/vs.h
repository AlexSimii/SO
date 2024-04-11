#ifndef VERSION_CONTROLLER
    #define VERSION_CONTROLLER

    #define INDENT 4
    #define HAPPY_CODE 200
    #define SAD_CODE (404 % 255)

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

    enum dir_states is_dir_watched(char *dir_path, char *CACHE_DIR);
    void save_snapshot(char *dir_path, char *CACHE_DIR);
    bool are_changes(char *dir_path, char *CACHE_DIR, bool save_newest);
    void track(char *dir_path, char *CACHE_DIR);
    pid_t generate_appropiate_process(char *dir_path, char *CACHE_DIR, pid_t main_pid);
    
#endif