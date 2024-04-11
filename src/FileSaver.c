#include <stdio.h>

#include <sys/types.h>
#include <sys/wait.h>

#include "vs.h"

/* 
cerinta laborator:
creez proces pt orice pt care are sens
./run dir1 dir2 dir3 cevarandom -o outDir
*/

void set_flags(int argc, char *argv[], char **CACHE_DIR, int *start, int *end)
{
    int found_cahe_dir = 0;
    *start = -1;
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

        if(*start != -1 && *end == -1)//found another flag aftre start, it s last index with  a dir
            *end = i - 1;

        if(argv[i][1] == 'd' || argv[i][1] == 'D') // -d -D flag
            *start = i + 1;
    }

    if(!found_cahe_dir)
    {
        *CACHE_DIR = (char *)malloc(16 * sizeof(char)); 
        is_null(*CACHE_DIR, ALOC_TEXT);
        strcpy(*CACHE_DIR, "FileSaverCache");
    }

    if(*start == -1)
    {
        *start = 1;
        *end = (before_first_flag == -1) ? argc - 1 : before_first_flag;
    }
    if(*end == -1)
        *end = argc;
}

int main(int argc, char *argv[])
{
    //de adaugat checks pt argiumente
    int start, end;// start < end
    char *CACHE_DIR = NULL; 
    pid_t main_pid = getpid();

    set_flags(argc, argv, &CACHE_DIR, &start, &end);

    // for(int i  = start; i < end; i ++)
    //     track(argv[i], CACHE_DIR);

    for(int i  = start; i < end; i ++)
        generate_appropiate_process(argv[i], CACHE_DIR, main_pid);

    int return_code = -1;
    pid_t finished_pid = 0;
    for(int i = start; i < end; i ++)
    {
        finished_pid = wait(&return_code);
        if(WIFEXITED(return_code))
            if(WEXITSTATUS(return_code) != HAPPY_CODE)
                printf("wait pid=%d: code=%d\n", finished_pid, WEXITSTATUS(return_code));
    }

    return 0;
}