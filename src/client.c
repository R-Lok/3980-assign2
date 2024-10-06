#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int  selectFilterChar(const char *str);
int  checkValidArgs(const char *str, char filterChar);
void printArgsFormat(void);

int main(int argc, char **argv)
{
    int         opt;
    const char *str        = NULL;
    char        filterChar = '\0';

    while((opt = getopt(argc, argv, ":i:f:")) != -1)
    {
        switch(opt)
        {
            case 'i':
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
    if(checkValidArgs(str, filterChar))
    {
        errno = EINVAL;
        perror("Invalid arguments");
        printArgsFormat();
        exit(EXIT_FAILURE);
    }

    printf("%s, %c", str, filterChar);
}

void printArgsFormat(void)
{
    printf("To run: ./build/client -i \"<text to transform>\" -f <upper/lower/null>");
}

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
