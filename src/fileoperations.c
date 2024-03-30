#include"fileoperations.h"

#include <stdio.h>
#include<errno.h>
#include<string.h>
#include<ctype.h>
#include <stdlib.h>

//system
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


struct stat get_i_node(const char *path)
{
    struct stat i_node;
    if(lstat(path, &i_node) < 0){
        char message[500];
        message[0] = '\0';
        strcat(message, "get_i_node() ");
        strcat(message, path);
        perror(message);
        exit(EXIT_FAILURE);
    }

    return i_node;
}

struct stat check_file_exists(const char *path)
{   
    struct stat i_node;
    if(lstat(path, &i_node) < 0){
        i_node.st_mode = ~__S_IFDIR;//nu e dir
    }
    return i_node;
}

















