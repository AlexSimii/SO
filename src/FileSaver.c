#include <stdio.h>
#include "vs.h"

/* 
cerinta laborator:
primesc oricate arg in lin dar nu mai mult de 10
nicun arg nu se v a repeta -- trebuei sa verific
prog prodcesasa dor dir, alte doc uri o sa fie ignorate
logica de captura de aplica turturor arg primite
prog actualizeaza snapshoturile pe toate dir urile

in cazul in care se vor inregistra schimbari
se compara cu versiunea veche a snapshotului
daca exitsa diferente sanpshotul vechi se actualizeaza

pe lanag intrari o sa avem un arg suplimetar care va fi dir e out
in care o sa fie toat snapshoturile intrarilor in lin d e comanda
./run dir1 dir2 dir3 cevarandom -o outDir
*/

void set_flags(int argc, char *argv[], char **CACHE_DIR, int *start, int *end)
{
    int found_dir_flag = 0;
    int found_cahe_dir = 0;
    *end = -1;
    int before_first_flag = -1;
    for(int i = 1; i < argc - 1; i ++)
    {
        if(!(argv[i][0] == '-'))
            continue;

        before_first_flag = (before_first_flag == -1) ? i : before_first_flag;

        if(argv[i][1] == 'O' || argv[i][1] == 'o') // -o -O flag
        {
            found_cahe_dir = 1;
            *CACHE_DIR = (char *)malloc(strlen(argv[i + 1]) * sizeof(char)); 
            is_null(*CACHE_DIR, ALOC_TEXT);
            strcpy(*CACHE_DIR, argv[i + 1]);
            i ++;
        }

        if(found_dir_flag == 1)//found another flag aftre start, it s last index with  a dir
            *end = i;

        if(argv[i][1] == 'd' || argv[i][1] == 'D') // -d -D flag
        {
            *start = i + 1;
            found_dir_flag = 1;
        }
    }

    if(!found_cahe_dir)
    {
        *CACHE_DIR = (char *)malloc(16 * sizeof(char)); 
        is_null(*CACHE_DIR, ALOC_TEXT);
        strcpy(*CACHE_DIR, "FileSaverCache");
    }

    if(!found_dir_flag)
    {
        *start = 1;
        *end = (before_first_flag == -1) ? argc - 1 : before_first_flag;
    }
    if(*end == -1)
        *end = argc;
}

int main(int argc, char *argv[])
{
    int start, end;// start < end
    char *CACHE_DIR = NULL;

    set_flags(argc, argv, &CACHE_DIR, &start, &end);

    return 0;

    if(argc == 2){
        //watch_directory(argv[1]);
        save_snapshot(argv[1], CACHE_DIR);
    }
    else
        if(argc == 1)
            printf("no directory entered\n");
        else
            printf("too many arguments\n");
    
    return 0;
}