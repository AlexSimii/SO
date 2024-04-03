#include "vs.h"

bool watch_directory(char *dir_path, const char *CACHE_DIR)
{   
    if(dir_path[strlen(dir_path) - 1] == '/') // GRESEALA poate are / pe untima poz dar dc e -1?!?!
        dir_path[strlen(dir_path) - 1] = '\0';//accept doar cai la directoare fara "/" la final
    // printf("watch_directory(): [%s]\n", dir_path);//func call

    Path_class watched_dir_path = make_path(dir_path);
    struct stat i_node = get_i_node(watched_dir_path);

    if(S_ISDIR(i_node.st_mode))
    {
        if(check_dir_is_tracked(dir_path, CACHE_DIR))
        {
            // printf("watch_directory(): [%s]\n", dir_path);//func call
            delete_path(watched_dir_path);
            return true;
        }
        else
        {
            // printf("Directory {%s} is untracked\n", watched_dir_path.path);//func call
            delete_path(watched_dir_path);
            return false;
        }
    }
    else{
        printf("An argument was given, but not a dir\n");
        delete_path(watched_dir_path);
        exit(EXIT_FAILURE);
    }
    return false;
}


void get_snapshot_and_store_it(const Path_class current_dir, int depth, int indent, Path_class snap_dir_path, const char *CACHE_DIR)
{
    struct stat i_node = get_i_node(current_dir);

    Cache_entry_class cache_entry;
    get_cache_entry_from_i_node(&cache_entry, i_node, current_dir, depth, indent);
    // printf("cache get_snapshot_and_store_it([%s], [%d], [%d], [%s])-> [%s]\n", current_dir.path, depth, indent, snap_dir_path.path ,cache_entry.text); // func call
    write_cache_entry_to_file(cache_entry, snap_dir_path, CACHE_DIR);

    DIR *directory = open_director(current_dir);
    struct dirent *first_entry = readdir(directory);
    while(first_entry != NULL)
    {
        if(first_entry->d_name[0] == '.' && (first_entry->d_name[1] == '\0' || (first_entry->d_name[1] == '.' && first_entry->d_name[2] == '\0')))
        {
            //self/dad refe
            first_entry = readdir(directory);
            continue;
        }
        Path_class entry = make_path(current_dir.path);
        expand_path(&entry, first_entry->d_name);
        printf("name %s\n",entry.path);

        if(is_dir(entry))
            get_snapshot_and_store_it(entry, depth + 1, indent, snap_dir_path, CACHE_DIR);
        else
        {
            struct stat file_i_node = get_i_node(entry);
            Cache_entry_class file_cache_entry;
            get_cache_entry_from_i_node(&file_cache_entry, file_i_node, entry, depth + 1, indent);
            write_cache_entry_to_file(file_cache_entry, snap_dir_path, CACHE_DIR);
            delete_cache_entry(file_cache_entry);
        }
        first_entry = readdir(directory);
        delete_path(entry);
    }
    delete_cache_entry(cache_entry);
}

void save_snapshot(char *dir_path, const char *CACHE_DIR)
{
    if(dir_path[strlen(dir_path) - 1] == '/')
        dir_path[strlen(dir_path) - 1] = '\0';//accept doar cai la directoare fara "/" la final
    
    // printf("save_snapshot(): %s\n", dir_path);//func call

    Path_class dir_path_to_save = make_path(dir_path);
    Path_class snap_dir_path = make_snap_dir_path(dir_path, CACHE_DIR);

    get_snapshot_and_store_it(dir_path_to_save, 0, INDENT, snap_dir_path, CACHE_DIR);

    delete_path(snap_dir_path);
    delete_path(dir_path_to_save);
}