#include "memory_checks.h"

void is_null(void* ptr, const char *text)
{
    if(ptr == NULL) 
    {
        printf("Error while %s memory\n", text);
        exit(EXIT_FAILURE);
    }
}