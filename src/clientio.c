#include "../include/clientio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int constructMsg(char filterChar, const char **str, char **msg)
{
    char  *tempmsg;
    size_t strlength = strlen(*(str));

    tempmsg = (char *)malloc(strlength + 2);
    if(tempmsg == NULL)
    {
        return 1;
    }
    *tempmsg = filterChar;
    strlcpy(tempmsg + 1, *(str), strlength + 1);    // strlength + 1 to account for NUL terminator.
    *msg = tempmsg;
    return 0;
}

int writeReq(int reqFd, const char *msg)
{
    size_t msgLength = strlen(msg);
    size_t nwrote    = 0;
    do
    {
        ssize_t twrote = write(reqFd, msg + nwrote, strlen(msg) - nwrote);
        if(twrote == -1)
        {
            perror("Error writing to request fifo\n");
            return 1;
        }
        nwrote += (size_t)twrote;
    } while(nwrote != msgLength);

    return 0;
}

int processResponse(int resFd, size_t strlength)
{
    ssize_t nread = 0;
    char   *buf   = (char *)malloc(strlength + 1);
    if(buf == NULL)
    {
        perror("malloc failed \n");
        return 1;
    }

    do
    {
        ssize_t tread;
        tread = read(resFd, buf + nread, (strlength - (size_t)nread));
        if(tread == -1)
        {
            perror("read failed\n");
            free(buf);
            return 1;
        }
        nread += tread;
    } while(nread != (ssize_t)strlength);
    buf[strlength] = '\0';

    printf("Response: %s\n", buf);
    free(buf);
    return 0;
}
