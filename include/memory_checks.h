#ifndef MEMORY_CHECKS_H
    #define MEMORY_CHECKS_H
    #define ALOC_TEXT "allocating"
    #define REALOC_TEXT "reallocating"

    #include <stdio.h>
    #include <stdlib.h>

    void is_null(void* ptr, const char *text);

#endif