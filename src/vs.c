#include "vs.h"



enum dir_states is_dir_watched(char *dir_path, char *CACHE_DIR)
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
            delete_path(&watched_dir_path);
            return watched;
        }
        else
        {
            // printf("Directory {%s} is untracked\n", watched_dir_path.path);//func call
            delete_path(&watched_dir_path);
            return unwatched;
        }
    }
    else{
        printf("An argument was given, but not a dir\n");
        delete_path(&watched_dir_path);
        return NOTdir;
    }
    return NOTpossible;
}

void get_snapshot(const Path_class current_dir, int depth, int indent, Snapshot *snap)
{
    //printf("dir called 1: [%s]\n",current_dir.path);
    struct stat i_node = get_i_node(current_dir);
    //printf("dir called 2: [%s]\n",current_dir.path);
    Cache_entry_class cache_entry;
    get_cache_entry_from_i_node(&cache_entry, i_node, current_dir, depth, indent);
    //printf("dir called 3: [%s]\n",current_dir.path);
    add_cache_entry(snap, cache_entry);
    //printf("dir called 4: [%s]\n",current_dir.path);

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
        delete_path(&entry);
    }
}

void save_snapshot(char *dir_path, char *CACHE_DIR)
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
    

    delete_path(&snap_dir_path);
    delete_path(&dir_path_to_save);
    delete_snapshot(&snap);
}

bool are_changes(char *dir_path, char *CACHE_DIR, bool save_newest)
{
    if(dir_path[strlen(dir_path) - 1] == '/')
        dir_path[strlen(dir_path) - 1] = '\0';
    
    Path_class dir_path_to_save = make_path(dir_path);
    Path_class snap_dir_path = make_snap_dir_path(dir_path, CACHE_DIR);

    Snapshot *snap_current = creeate_snapshot();
    get_snapshot(dir_path_to_save, 0, INDENT, snap_current);

    Snapshot *last_save = creeate_snapshot();
    load_snapshot(last_save, snap_dir_path);

    bool answ = is_changed(last_save, snap_current);

    if(save_newest && answ)
    {
        if(remove(snap_dir_path.path) != 0)
            printf("Unable to remove last save");
        for(int i = 0; i < snap_current->nr_elem; i ++)
            write_cache_entry_to_file(snap_current->arr[i], snap_dir_path, CACHE_DIR);
    }


    delete_path(&snap_dir_path);
    delete_path(&dir_path_to_save);

    return answ;
}

void track(char *dir_path, char *CACHE_DIR)
{
    switch (is_dir_watched(dir_path, CACHE_DIR))
    {
    case unwatched:
        printf("Dir %s is untracked, first save was made\n", dir_path);
        save_snapshot(dir_path, CACHE_DIR);
        break;
    case watched:
        if(are_changes(dir_path, CACHE_DIR, true))
            printf("Changes were found changes in /%s, a new version was saved\n", dir_path);
        else
            printf("No changes found in /%s\n", dir_path);
        break;
    case NOTdir:
        printf("/%s is not a valid dir\n", dir_path);
        break;
    case NOTpossible:
        printf("Whe provided entry [%s] is  WHAAAT?!?!? \n", dir_path);
        break;
    }
}

pid_t generate_appropiate_process(char *dir_path, char *CACHE_DIR, pid_t main_pid)
{
    pid_t pid, where_am_i;

    switch (is_dir_watched(dir_path, CACHE_DIR))
    {
    case unwatched:
        printf("Dir %s is untracked, first save was made\n", dir_path);
        
        pid = fork();
        pid_t where_am_i = getpid();

        if(where_am_i != main_pid)//nu sunt in main
        {
            //in child pid o sa fie 0
            if(pid < 0)
            {
                printf("Error in fork, unwatched dir %s\n", dir_path);
                exit(EXIT_FAILURE);
            }
            save_snapshot(dir_path, CACHE_DIR);
            exit(HAPPY_CODE);
        }
        else//sunt in main
        {
            //printf("pid %d creeated\n", pid);
            return pid;//returnez procesul dat
        }
            
        break;
    case watched:
        pid = fork();
        where_am_i = getpid();

        if(where_am_i != main_pid)
        {
            if(pid < 0)
            {
                //printf("Error in fork, watched dir %s\n", dir_path);
                exit(EXIT_FAILURE);
            }
            if(are_changes(dir_path, CACHE_DIR, true))
                printf("Changes were found changes in /%s, a new version was saved\n", dir_path);
            else
                printf("No changes found in /%s\n", dir_path);
            exit(HAPPY_CODE);
        }
        else
        {
            //printf("pid %d creeated\n", pid);
            return pid;
        }
        break;
    case NOTdir:
        printf("/%s is not a valid dir\n", dir_path);
        return -1;
        break;
    case NOTpossible:
        printf("Whe provided entry [%s] is  WHAAAT?!?!? \n", dir_path);
        return -1;
        break;
    }
    exit(SAD_CODE);
}


