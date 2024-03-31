#ifndef CACHE_ENTRY_H
    #define CACHE_ENTRY_H
    #define CACHE_ENTRY_SIZE 500

    #include "path.h"
    #include "memory_checks.h"
    #include "fileoperations.h"

    #include <stdio.h>
    #include <stdbool.h>
    #include <stdlib.h>
    #include <string.h>

    //system
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>

    typedef struct cache_entry{
        char *text;
    }Cache_entry_class;

    void edit_cache_entry(Cache_entry_class *cache_entry, const char *new_entry, bool last_entry);
    void get_cache_entry_from_i_node(Cache_entry_class *cache_entry, struct stat i_node, const Path_class dir_path, int depth, int indent);
    void write_cache_entry_to_file(Cache_entry_class cache_entry, Path_class snap_dir_path);

#endif