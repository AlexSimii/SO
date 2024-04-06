#include "cache_entry.h"


void write_cache_entry_to_file(Cache_entry_class cache_entry, Path_class snap_dir_path, const char *CACHE_DIR)
{
    //fac out dir daca nu e
    Path_class cahe_dir_temp = make_path(CACHE_DIR);
    if(!is_dir(cahe_dir_temp))
    {
        mkdir(cahe_dir_temp.path, 0700);
    }
    delete_path(cahe_dir_temp);
    
    int file_to_write_cache = open_snapshot_file_for_cache(snap_dir_path);
    int nr_of_bytes = strlen(cache_entry.text) * sizeof(char);
    if(write(file_to_write_cache, cache_entry.text, nr_of_bytes) != nr_of_bytes)
    {
        printf("nu  s o scris cat trebe/nu  so scris %s\n", snap_dir_path.path);// func call
        perror("in write cahche\n");
        exit(EXIT_FAILURE);
    }
    
    if(close(file_to_write_cache) < 0)
    {
        printf("dupa ce s o scris in file nu s o inchis %s\n", snap_dir_path.path);// func call
        exit(EXIT_FAILURE);
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
    temp[depth * indent] = '\0';
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

void delete_cache_entry(Cache_entry_class *c)
{
    if(c->text != NULL)
        free(c->text);
}


Snapshot * creeate_snapshot()
{
    Snapshot *s = (Snapshot *)malloc(1 * sizeof(Snapshot));
    is_null(s, ALOC_TEXT);
    s-> arr = (Cache_entry_class *)malloc(sizeof(Cache_entry_class) * SNAPSHOT_CHUNK_SIZE);
    is_null(s->arr, ALOC_TEXT);
    s->nr_elem = 0;
    s->size = SNAPSHOT_CHUNK_SIZE;

    return s;
}

void increase_size(Snapshot **s)
{
    Cache_entry_class *temp = (Cache_entry_class *)realloc((*s)->arr, sizeof(Cache_entry_class) * ((*s)->size + SNAPSHOT_CHUNK_SIZE));
    is_null(temp, REALOC_TEXT);
    (*s)->arr = temp;
    (*s)->size += SNAPSHOT_CHUNK_SIZE;
}

void add_cache_entry(Snapshot *s, Cache_entry_class entry)
{
    if(s->nr_elem < s->size)
    {
        s->arr[s->nr_elem] = entry;
        (s->nr_elem) ++;
    }
    else{
        increase_size(&s);
        s->arr[s->nr_elem] = entry;
        (s->nr_elem) ++;
    }
}

void delete_snapshot(Snapshot **s)
{
    for(int i = 0; i < (*s)->nr_elem; i ++)
    {
        delete_cache_entry(&((*s)->arr[i]));
    }
    free((*s)->arr);
    free(*s);
}


