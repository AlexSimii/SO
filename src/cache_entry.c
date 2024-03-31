#include "cache_entry.h"


void write_cache_entry_to_file(Cache_entry_class cache_entry, Path_class snap_dir_path)
{
    printf("snap_dir_path = [%s]\n", snap_dir_path.path);
    if(!check_file_is_dir(snap_dir_path))
    {
        
    }

}


void init_cache_entry(Cache_entry_class *cache_entry)
{
    cache_entry->text = (char *)malloc(1 * sizeof(char));
    is_null(cache_entry->text, ALOC_TEXT);
    cache_entry->text[0] = '\0';
}

void get_cache_entry_from_i_node(Cache_entry_class *cache_entry, struct stat i_node, const Path_class dir_path, int depth, int indent)
{
    init_cache_entry(cache_entry);

    char temp[CACHE_ENTRY_SIZE];
    for(int i = 0; i < depth * indent; i ++){
        temp[i] = ' ';
    }
    temp[depth * indent + 1] = '\0';
    edit_cache_entry(cache_entry, temp, false);//indentation

    sprintf(temp, "%ld", i_node.st_ino);//st_ino
    edit_cache_entry(cache_entry, temp, false);

    sprintf(temp, "%d", i_node.st_mode);//st_mode
    edit_cache_entry(cache_entry, temp, false);

    sprintf(temp, "%d", i_node.st_uid);//st_uid
    edit_cache_entry(cache_entry, temp, false);

    sprintf(temp, "%ld", i_node.st_size);//st_size
    edit_cache_entry(cache_entry, temp, false);

    sprintf(temp, "%ld", i_node.st_ino);//st_blksize
    edit_cache_entry(cache_entry, temp, false);

    edit_cache_entry(cache_entry, dir_path.path, true);//file_name
}

void edit_cache_entry(Cache_entry_class *cache_entry, const char *new_entry, bool last_entry)
{
    int extra;
    if(!last_entry)
        extra = 2;// [, ]
    else
        extra = 1;// [\n]

    char *temp = (char *)realloc(cache_entry->text, (strlen(cache_entry->text) + strlen(new_entry) + extra) * sizeof(char));
    is_null(temp, REALOC_TEXT);
    cache_entry->text = temp;

    strcat(cache_entry->text, new_entry);
    if(!last_entry)
        strcat(cache_entry->text, ", ");
    else
        strcat(cache_entry->text, "\n");
}




