#include <stdio.h>
#include "vs.h"


int main( int argc, char *argv[] )  {

    if(argc == 2){
        //watch_directory(argv[1]);
        save_snapshot(argv[1]);
    }
    else
        if(argc == 1)
            printf("no directory entered\n");
        else
            printf("too many arguments\n");
    
    return 0;
}