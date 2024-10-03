#include <stdlib.h>
#include <fcntl.h>

#define reqFifo "/tmp/request"
#define resFifo "/tmp/response"

int main(void)
{
    int ret;
    ret = EXIT_SUCCESS;
    int8_t reqFd = open(reqFifo, O_RDONLY | O_CLOEXEC);
    if (reqFd == -1) {
        perror("open reqFifo");
        goto fail_req;
    }

    int8_t resFd = open(resFifo, O_WRONLY | O_CLOEXEC);
    if(resFd == -1) {
        perror("open resFifo");
        goto fail_res;
    }



    fail_res:
    close(reqFd);
    fail_req:
    ret = EXIT_FAILURE;
    return ret;
}
