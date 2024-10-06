#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int  selectFilterChar(char *str);
void printArgsFormat();

int main(int argc, char **argv)
{
    int   opt;
    char *str = NULL;
    char  filterChar;

    while((opt = getopt(argc, argv, ":i:f:")))
    {
        switch(opt)
        {
            case 'i':
                printf("Input string: %s", optarg);
                str = optarg;
                break;
            case 'f':
                filterChar = (char)selectFilterChar(optarg);
                break;
            case ':':
                errno = EINVAL;
                perror("Required argument for flag option is missing");
                printArgsFormat();
                exit(EXIT_FAILURE);
            default:
                printf("Useless flag entered: %c, proceeding..", optopt);
        }
    }
}

void printArgsFormat()
{
    printf("To run: ./build/client -i \"<Text>\" -f <upper/lower/null>");
}

int selectFilterChar(char *str)
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
