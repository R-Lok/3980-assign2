#ifndef CLIENTIO_H
#define CLIENTIO_H
#include <stdlib.h>

int constructMsg(char filterChar, const char **str, char **msg);
int writeReq(int reqFd, const char *msg);
int processResponse(int resFd, size_t strlength);

#endif
