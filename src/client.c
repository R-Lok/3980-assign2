#include "args.h"
#include "clientio.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define reqFifo "/tmp/request"
#define resFifo "/tmp/response"

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
                perror("Required argument for flag option is missing\n");
                printArgsFormat();
                exit(EXIT_FAILURE);
            default:
                printf("Unrecognized flag entered: %c, terminating.\n", optopt);
                printArgsFormat();
                exit(EXIT_FAILURE);
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
        perror("Filed to open request fifo\n");
        ret = EXIT_FAILURE;
        goto done;
    }

    resFd = open(resFifo, O_RDONLY | O_CLOEXEC);
    if(resFd == -1)
    {
        perror("Failed to open response fifo\n");
        ret = EXIT_FAILURE;
        goto fail_openres;
    }

    if((constructMsg(filterChar, &str, &message)) == 1)
    {
        goto fail_malloc;
    }

    if(writeReq(reqFd, message))
    {
        goto cleanup;
    }

    processResponse(resFd, strlen(str));

cleanup:
    free(message);
fail_malloc:
    close(resFd);
fail_openres:
    close(reqFd);
done:
    return ret;
}
