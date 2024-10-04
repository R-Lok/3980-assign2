#include "filters.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int processText(int inputFd, int outputFd, char filterChar, int *err)
{
    char        buffer;    // Processing one char at the same time
    char        transformedChar;
    ssize_t     readRes;
    filter_func filter;

    filter = selectFilter(filterChar);

    while(read(inputFd, &buffer, 1) != 0)
    {
        ssize_t writeRes;
        if(readRes == -1)
        {
            *err = errno;
            perror("Read error");
            return (EXIT_FAILURE);
        }
        transformedChar = func(buffer);
        writeRes        = write(outputFd, &transformedChar, 1);
        if(writeRes == -1)
        {
            *err = errno;
            perror("Write error");
            return (EXIT_FAILURE);
        }
    }
    return EXIT_SUCCESS;
}