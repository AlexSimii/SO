#include <stdio.h>
#include "vs.h"


int main( int argc, char *argv[] )  {

    if(argc == 2){
        watch_directory(argv[1]);
    }
    else
        printf("no directory entered\n");

}