#include"fileoperations.h"

struct stat get_i_node(const Path_class path)
{
    struct stat i_node;
    if(lstat(path.path, &i_node) < 0){
        char message[500];
        message[0] = '\0';
        strcat(message, "get_i_node() ");
        strcat(message, path.path);
        perror(message);
        exit(EXIT_FAILURE);
    }

    return i_node;
}

bool check_file_is_dir(const Path_class path)
{   
    struct stat i_node;
    if(lstat(path.path, &i_node) < 0){
        return false;
    }
    return (S_ISDIR(i_node.st_mode) != 0);
}

bool check_dir_is_tracked(const char *dir_path, const char *cache_dir)
{
    Path_class snap_dir_path = make_snap_dir_path(dir_path, cache_dir);

    if(check_file_is_dir(snap_dir_path))
        return true;
    return false;
}















