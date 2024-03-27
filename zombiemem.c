/*
In a subthread, allocate and touch 1 GiB of memory.
The main thread exits and will show up as a zombie.

Compile: gcc zombiemem.c -o zombiemem
*/

// needed for gettid
#define _GNU_SOURCE

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void* spin(__attribute__((__unused__)) void* arg)
{
    const int size = 1024*1024*1024;
    char* p = malloc(size);
    if(!p) {
        perror("malloc failed");
        exit(1);
    }
    // the memset ensures the memory is actually allocated
    memset(p, 123, size);
    printf("sub thread (pid %d) now uses 1 GiB of memory. Sleeping.\n", gettid());
    while(1) {
        sleep(3600);
    }
    return NULL;
}

int main()
{
    pthread_t thread;
    pthread_create(&thread, NULL, &spin, NULL);

    printf("main thread (pid %d) exiting\n", gettid());
    pthread_exit(NULL);
}
