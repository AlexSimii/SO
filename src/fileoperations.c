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

bool is_dir(const Path_class path)
{   
    struct stat i_node;
    if(lstat(path.path, &i_node) < 0){
        return false;
    }
    return (S_ISDIR(i_node.st_mode) != 0);
}

bool is_file(const Path_class path)
{   
    struct stat i_node;
    if(lstat(path.path, &i_node) < 0){
        return false;
    }
    return (S_ISREG(i_node.st_mode) != 0);
}

bool check_dir_is_tracked(const char *dir_path, const char *cache_dir)
{
    Path_class snap_dir_path = make_snap_dir_path(dir_path, cache_dir);

    printf("check_dir_is_tracked(%s, %s)\n", dir_path, cache_dir);
    printf("at path : %s\n", snap_dir_path.path);
    return (is_file(snap_dir_path) == true);
}

int open_snapshot_file_for_cache(Path_class p)
{
    int ans = open(p.path, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP);
    if(ans < 0)
    {
        printf("Error opening file %s , func : deschide_snapshot_file_for_cache()\n", p.path);
        exit(EXIT_FAILURE);
    }
    return  ans;
}


DIR *open_director(Path_class p)
{
    DIR *ans = opendir(p.path);
    if(ans == NULL)
    {
        printf("Directorul %s nu a putut sa fie deschis\n", p.path);
        exit(EXIT_FAILURE);
    }
    return ans;
}














