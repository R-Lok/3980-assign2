#include "../include/io.h"
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define reqFifo "/tmp/request"
#define resFifo "/tmp/response"

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
static volatile sig_atomic_t signal_flag = 0;    // This non-const global is needed if graceful shutdown involves closing file descriptors,
                                                 // since signal handler functions can only take the signal number, there is no way to pass
                                                 // the file descriptors for closing in the handler logic. So, need to make use of a global flag to know to
                                                 // goto cleanup logic.

void handle_signal(int signal);

int main(void)
{
    int resFd;
    int reqFd;
    int ret = EXIT_SUCCESS;

    printf("Starting server...\n");
    signal(SIGINT, handle_signal);

    reqFd = open(reqFifo, O_RDONLY | O_CLOEXEC);
    if(reqFd == -1)
    {
        if(errno == EINTR)
        {
            printf("SIGINT signal received, gracefully terminating..\n");
            exit(EXIT_SUCCESS);
        }
        perror("error on open reqFifo\n");
        ret = EXIT_FAILURE;
        goto fail_req;
    }

    resFd = open(resFifo, O_WRONLY | O_CLOEXEC);
    if(resFd == -1)
    {
        perror("error on open resFifo\n");
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

        if(signal_flag == SIGINT)
        {
            printf("SIGINT signal received, gracefully terminating..\n");
            goto cleanup;
        }

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
cleanup:
    if(close(resFd) == -1)
    {
        perror("error closing response fifo");
        ret = EXIT_FAILURE;
    }
fail_res:
    if(close(reqFd) == -1)
    {
        perror("error closing request fifo");
        ret = EXIT_FAILURE;
    }
fail_req:
    return ret;
}

void handle_signal(int signal)
{
    signal_flag = signal;
}
