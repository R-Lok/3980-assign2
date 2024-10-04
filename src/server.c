#include "../include/io.h"
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define reqFifo "/tmp/request"
#define resFifo "/tmp/response"

int main(void)
{
    int ret;
    int reqFd;
    int resFd;

    reqFd = open(reqFifo, O_RDONLY | O_CLOEXEC);
    if(reqFd == -1)
    {
        perror("open reqFifo");
        ret = EXIT_FAILURE;
        goto fail_req;
    }

    resFd = open(resFifo, O_WRONLY | O_CLOEXEC);
    if(resFd == -1)
    {
        perror("open resFifo");
        ret = EXIT_FAILURE;
        goto fail_res;
    }

    while(true)
    {
        char filterChar;
        int  err;

        ssize_t readRes = read(reqFd, &filterChar, 1);
        if(readRes == -1)
        {
            perror("read reqFifo");
            ret = EXIT_FAILURE;
            goto cleanup;
        }

        if(processText(reqFd, resFd, filterChar, &err))
        {
            errno = err;
            ret   = EXIT_FAILURE;
            perror("read/write error in processText");
            goto cleanup;
        }
    }
    ret = EXIT_SUCCESS;
cleanup:
    close(resFd);
fail_res:
    close(reqFd);
fail_req:
    return ret;
}
