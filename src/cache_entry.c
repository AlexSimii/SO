#include "cache_entry.h"


void write_cache_entry_to_file(Cache_entry_class cache_entry, Path_class snap_dir_path, char *CACHE_DIR)
{
    //fac out dir daca nu e
    Path_class cahe_dir_temp = make_path(CACHE_DIR);
    if(!is_dir(cahe_dir_temp))
    {
        mkdir(cahe_dir_temp.path, 0700);
    }
    delete_path(&cahe_dir_temp);
    
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
    cache_entry->text = (char *)malloc(10 * sizeof(char));//excess
    is_null(cache_entry->text, ALOC_TEXT);
    cache_entry->text[0] = '\0';
}

void get_cache_entry_from_i_node(Cache_entry_class *cache_entry, struct stat i_node, Path_class dir_path, int depth, int indent)
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

void edit_cache_entry(Cache_entry_class *cache_entry, char *new_entry, bool last_entry)
{
    int extra;
    if(!last_entry)
        extra = 2;// [, ]
    else
        extra = 1;// [\n]

    char *temp = (char *)realloc(cache_entry->text, (strlen(cache_entry->text) + strlen(new_entry) + extra + 1) * sizeof(char));
    is_null(temp, REALOC_TEXT);

    cache_entry->text = temp;

    strcat(cache_entry->text, new_entry);
    if(!last_entry)
        strcat(cache_entry->text, ", ");
    else
        strcat(cache_entry->text, "\n");
}

void make_cache_entry_text(Cache_entry_class *cache_entry, char *text)
{
    char *temp = (char *)realloc(cache_entry->text, (strlen(cache_entry->text) + strlen(text) + 1) * sizeof(char));
    is_null(temp, REALOC_TEXT);

    cache_entry->text = temp;

    strcat(cache_entry->text, text);
    strcat(cache_entry->text, "\n");
}

void delete_cache_entry(Cache_entry_class *c)
{
    if(c->text != NULL)
        free(c->text);
    c->text = NULL;
}

Snapshot * creeate_snapshot()
{
    Snapshot *s = (Snapshot *)malloc(sizeof(Snapshot));
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
    if((*s)->arr != NULL)
        free((*s)->arr);
    (*s) -> arr = NULL;
    if(*s != NULL)
        free(*s);
    *s = NULL;
}

void load_snapshot(Snapshot *loaded, Path_class snap_dir_path)
{
    int text_size = LOAD_CHUNK_SIZE + 1;

    char *text = (char *)malloc((sizeof(char) + 1)* LOAD_CHUNK_SIZE);
    char *chunk = (char *)malloc((sizeof(char) + 1)* LOAD_CHUNK_SIZE);

    is_null(text, ALOC_TEXT);
    is_null(chunk, ALOC_TEXT);

    text[0] = '\0';

    int filedesc = open_snapshot_read(snap_dir_path);
    int read_status = read(filedesc, chunk, LOAD_CHUNK_SIZE);
    while(read_status == LOAD_CHUNK_SIZE)
    {
        chunk[LOAD_CHUNK_SIZE] = '\0';
        strcat(text, chunk);
        text_size += LOAD_CHUNK_SIZE;
        char *temp1 = (char *)realloc(text, text_size*sizeof(char));
        is_null(temp1, REALOC_TEXT);
        text = temp1;
        read_status = read(filedesc, chunk, LOAD_CHUNK_SIZE);
    }
    if(read_status < 0)
    {
        printf("error during reading file %s\n", snap_dir_path.path);
        exit(EXIT_FAILURE);
    }
    if(read_status < LOAD_CHUNK_SIZE)
    {
        chunk[read_status] = '\0';
        strcat(text, chunk);
    }

    //printf("txt:[\n%s\n]\n",text);

    chunk = strtok(text, "\n");
    while (chunk != NULL) {
        //printf("chunk:[%s]\n",chunk);

        Cache_entry_class temp;
        init_cache_entry(&temp);
        make_cache_entry_text(&temp, chunk);
        add_cache_entry(loaded, temp);

        chunk = strtok(NULL, "\n");
    }
    if(chunk != NULL)
        free(chunk);
    chunk = NULL;
    
    if(text != NULL)
        free(text);
    text = NULL;
}

bool is_changed(Snapshot *old, Snapshot *new)
{
    
    if(old->nr_elem != new->nr_elem)
        return true;

    for(int i = 0; i < old ->nr_elem; i ++)
        if(strcmp(old->arr[i].text, new->arr[i].text) != 0)
        {
            ///printf("is_changed\n");
            ///printf("old:[%s]\n", old->arr[i].text);
            ///printf("new:[%s]\n", new->arr[i].text);
            return true;
        }

    return false;
}



