#ifndef FILE_OPERATIONS
    #define FILE_OPERATIONS

    struct stat get_i_node(const char *path);
    struct stat check_file_exists(const char *path);
#endif