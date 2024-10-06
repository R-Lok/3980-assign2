#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define reqFifo "/tmp/request"
#define resFifo "/tmp/response"

int  selectFilterChar(const char *str);
int  checkValidArgs(const char *str, char filterChar);
void printArgsFormat(void);

int constructMsg(char filterChar, const char **str, char **msg);

int main(int argc, char **argv)
{
    int         opt;
    const char *str        = NULL;
    char        filterChar = '\0';
    int         reqFd;
    int         resFd;
    int         ret = EXIT_SUCCESS;
    char       *message;

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

    printf("%s, %c\n", str, filterChar);

    reqFd = open(reqFifo, O_WRONLY | O_CLOEXEC);
    if(reqFd == -1)
    {
        perror("Filed to open request fifo");
        ret = EXIT_FAILURE;
        goto done;
    }

    resFd = open(resFifo, O_RDONLY | O_CLOEXEC);
    if(resFd == -1)
    {
        perror("Failed to open response fifo");
        ret = EXIT_FAILURE;
        goto fail_openres;
    }

    if((constructMsg(filterChar, &str, &message)) == 1)
    {
        goto cleanup;
    }

    free(message);

cleanup:
    close(resFd);
fail_openres:
    close(reqFd);
done:
    return ret;
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

int constructMsg(char filterChar, const char **str, char **msg)
{
    char  *tempmsg;
    size_t strlength = strlen(*(str));
    printf("%zu", strlength);

    tempmsg = (char *)malloc(strlength + 2);
    if(tempmsg == NULL)
    {
        return 1;
    }
    *tempmsg = filterChar;
    strlcpy(tempmsg + 1, *(str), strlength + 1); //strlength + 1 to account for NUL terminator.
    *msg = tempmsg;
    return 0;
}
