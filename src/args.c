#include "args.h"
#include <stdio.h>
#include <string.h>

int selectFilterChar(const char *str)
{
    if(strcasecmp(str, "upper") == 0)
    {
        return 'u';
    }
    if(strcasecmp(str, "lower") == 0)
    {
        return 'l';
    }
    if(strcasecmp(str, "null") == 0)
    {
        return 'n';
    }
    return 0;    // charcode for NUL
}

int checkValidArgs(const char *str, char filterChar)
{
    if(!str || filterChar == '\0')
    {
        return 1;
    }
    return 0;
}

void printArgsFormat(void)
{
    printf("To run: ./build/client -i \"<text to transform>\" -f <upper/lower/null>\n");
}
