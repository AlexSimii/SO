#include "vs.h"

void watch_directory(const char *dir_path)
{   
    //printf("watch_directory(): %s\n", dir_path);//func call

    Path_class watched_dir_path = make_path(dir_path);
    struct stat i_node = get_i_node(watched_dir_path);

    if(S_ISDIR(i_node.st_mode))
    {
        if(check_dir_is_tracked(dir_path, CACHE_DIR))
            printf("Directory {%s} is tracked\n", watched_dir_path.path);
        else
            printf("Directory {%s} is untracked\n", watched_dir_path.path);
    }
    else{
        printf("An argument was given, but not a dir\n");
        delete_path(watched_dir_path);
        exit(EXIT_FAILURE);
    }
    delete_path(watched_dir_path);
}


void get_snapshot_and_store_it(const Path_class dir_path, int depth, int indent, Path_class snap_dir_path)
{
    struct stat i_node = get_i_node(dir_path);

    Cache_entry_class cache_entry;
    get_cache_entry_from_i_node(&cache_entry, i_node, dir_path, depth, indent);
    printf("cache get_snapshot_and_store_it([%s], [%d], [%d], [%s])-> [%s]\n", dir_path.path, depth, indent, snap_dir_path.path ,cache_entry.text); // func call

    write_cache_entry_to_file(cache_entry, snap_dir_path);
    
}

void save_snapshot(const char *dir_path)
{
    //printf("save_snapshot(): %s\n", dir_path);//func call

    Path_class dir_path_to_save = make_path(dir_path);
    Path_class snap_dir_path = make_snap_dir_path(dir_path, CACHE_DIR);

    get_snapshot_and_store_it(dir_path_to_save, 0, INDENT, snap_dir_path);

    delete_path(snap_dir_path);
    delete_path(dir_path_to_save);
}