#include "../include/io.h"
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define reqFifo "/tmp/request"
#define resFifo "/tmp/response"

int main(void)
{
    int resFd;
    int reqFd;
    int ret = EXIT_SUCCESS;

    printf("Starting server...\n");

    reqFd = open(reqFifo, O_RDONLY | O_CLOEXEC);
    if(reqFd == -1)
    {
        perror("open reqFifo\n");
        ret = EXIT_FAILURE;
        goto fail_req;
    }

    resFd = open(resFifo, O_WRONLY | O_CLOEXEC);
    if(resFd == -1)
    {
        perror("open resFifo\n");
        ret = EXIT_FAILURE;
        goto fail_res;
    }

    while(true)
    {
        char        filterChar;
        int         err;
        int         threadRes;
        pthread_t   thread;
        TextHandler details;
        ssize_t     readRes;

        readRes = read(reqFd, &filterChar, 1);
        if(readRes == -1)
        {
            perror("read reqFifo\n");
            ret = EXIT_FAILURE;
            goto cleanup;
        }

        if(readRes != 0)
        {
            details.inputFd    = reqFd;
            details.outputFd   = resFd;
            details.filterChar = filterChar;
            details.err        = &err;

            threadRes = pthread_create(&thread, NULL, processText, &details);
            if(threadRes == -1)
            {
                perror("Failed to create thread");
                ret = EXIT_FAILURE;
                goto cleanup;
            }

            threadRes = pthread_join(thread, NULL);
            if(threadRes == -1)
            {
                perror("Failed to join thread");
                ret = EXIT_FAILURE;
                goto cleanup;
            }

            if(details.threadExitVal)
            {
                ret = EXIT_FAILURE;
                goto cleanup;
            }
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
