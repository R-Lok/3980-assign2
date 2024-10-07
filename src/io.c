#include "io.h"
#include "filters.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *processText(void *textDetails)
{
    TextHandler *td;
    char         buffer;    // Processing one char at the same time
    char         transformedChar;
    ssize_t      readRes;
    filter_func  filter;

    td                = (TextHandler *)textDetails;
    td->threadExitVal = 0;

    filter = selectFilter(td->filterChar);

    while((readRes = read(td->inputFd, &buffer, 1)) != 0)
    {
        ssize_t writeRes;
        if(readRes == -1)
        {
            *(td->err) = errno;
            perror("Read error\n");
            td->threadExitVal = 1;
            return NULL;
        }
        transformedChar = filter(buffer);
        writeRes        = write(td->outputFd, &transformedChar, 1);
        if(writeRes == -1)
        {
            *(td->err) = errno;
            perror("Write error\n");
            td->threadExitVal = 1;
            return NULL;
        }
    }
    printf("Done sending response.\n");
    fflush(stdout);
    return NULL;
}
