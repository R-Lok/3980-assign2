#ifndef IO_H
#define IO_H

// cppcheck thinks I am not using these members at all, when I infact am, in the implementation of processText in io.c
typedef struct TextHandler
{
    // cppcheck-suppress unusedStructMember
    int inputFd;
    // cppcheck-suppress unusedStructMember
    int outputFd;
    // cppcheck-suppress unusedStructMember
    char filterChar;
    // cppcheck-suppress unusedStructMember
    int *err;
    // cppcheck-suppress unusedStructMember
    int threadExitVal;
} TextHandler;

void *processText(void *textDetails);

#endif
