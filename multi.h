#ifndef MULTI_H
#define MULTI_H
typedef struct
{
    char *group;
    int port;
} list_arg;

#define MULTI_QUEUE_NAME "multi"
#define SINGLE_QUEUE_NAME "single"

void *multilistner(void *);
void *multisender(void *);
void *udplistner(void *);
void *udpsender(void *);

#define MSGBUFSIZE 256

#endif
