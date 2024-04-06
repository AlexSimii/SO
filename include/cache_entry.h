#ifndef CACHE_ENTRY_H
    #define CACHE_ENTRY_H
    #define CACHE_ENTRY_SIZE 500
    #define SNAPSHOT_CHUNK_SIZE 10//greseala
    #define LOAD_CHUNK_SIZE 50
    
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

    typedef struct snap{
        Cache_entry_class *arr;
        int nr_elem;
        int size;
    }Snapshot;

    void edit_cache_entry(Cache_entry_class *cache_entry, char *new_entry, bool last_entry);
    void get_cache_entry_from_i_node(Cache_entry_class *cache_entry, struct stat i_node, Path_class dir_path, int depth, int indent);
    void write_cache_entry_to_file(Cache_entry_class cache_entry, Path_class snap_dir_path, char *CACHE_DIR);
    void delete_cache_entry(Cache_entry_class *c);

    Snapshot * creeate_snapshot();
    void increase_size(Snapshot **s);
    void add_cache_entry(Snapshot *s, Cache_entry_class entry);
    void delete_snapshot(Snapshot **s);

    void load_snapshot(Snapshot *loaded, Path_class snap_dir_path);
    bool is_changed(Snapshot *old, Snapshot *new);
#endif