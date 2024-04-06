#include "vs.h"

enum dir_states is_dir_watched(char *dir_path, const char *CACHE_DIR)
{   
    if(dir_path[strlen(dir_path) - 1] == '/')
        dir_path[strlen(dir_path) - 1] = '\0';
    // printf("watch_directory(): [%s]\n", dir_path);//func call

    Path_class watched_dir_path = make_path(dir_path);
    struct stat i_node = get_i_node(watched_dir_path);

    if(S_ISDIR(i_node.st_mode))
    {
        if(check_dir_is_tracked(dir_path, CACHE_DIR))
        {
            // printf("watch_directory(): [%s]\n", dir_path);//func call
            delete_path(watched_dir_path);
            return watched;
        }
        else
        {
            // printf("Directory {%s} is untracked\n", watched_dir_path.path);//func call
            delete_path(watched_dir_path);
            return unwatched;
        }
    }
    else{
        printf("An argument was given, but not a dir\n");
        delete_path(watched_dir_path);
        return NOTdir;
    }
    return NOTpossible;
}


void get_snapshot(const Path_class current_dir, int depth, int indent, Snapshot *snap)
{
    struct stat i_node = get_i_node(current_dir);

    Cache_entry_class cache_entry;
    get_cache_entry_from_i_node(&cache_entry, i_node, current_dir, depth, indent);
    add_cache_entry(snap, cache_entry);

    DIR *directory = open_director(current_dir);
    struct dirent *first_entry = readdir(directory);
    while(first_entry != NULL)
    {
        if(first_entry->d_name[0] == '.' && (first_entry->d_name[1] == '\0' || (first_entry->d_name[1] == '.' && first_entry->d_name[2] == '\0')))
        {
            //self/dad reference
            first_entry = readdir(directory);
            continue;
        }
        Path_class entry = make_path(current_dir.path);
        expand_path(&entry, first_entry->d_name);

        if(is_dir(entry))
            get_snapshot(entry, depth + 1, indent, snap);
        else
        {
            struct stat file_i_node = get_i_node(entry);
            Cache_entry_class file_cache_entry;
            get_cache_entry_from_i_node(&file_cache_entry, file_i_node, entry, depth + 1, indent);
            add_cache_entry(snap, file_cache_entry);
        }
        first_entry = readdir(directory);
        delete_path(entry);
    }
}

void save_snapshot(char *dir_path, const char *CACHE_DIR)
{
    if(dir_path[strlen(dir_path) - 1] == '/')
        dir_path[strlen(dir_path) - 1] = '\0';//accept doar cai la directoare fara "/" la final
    
    // printf("save_snapshot(): %s\n", dir_path);//func call

    Path_class dir_path_to_save = make_path(dir_path);
    Path_class snap_dir_path = make_snap_dir_path(dir_path, CACHE_DIR);

    Snapshot *snap = creeate_snapshot();
    get_snapshot(dir_path_to_save, 0, INDENT, snap);

    for(int i = 0; i < snap->nr_elem; i ++)
        write_cache_entry_to_file(snap->arr[i], snap_dir_path, CACHE_DIR);
    

    delete_path(snap_dir_path);
    delete_path(dir_path_to_save);
    delete_snapshot(&snap);
}