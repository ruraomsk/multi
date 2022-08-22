
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> // for sleep()

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <mqueue.h>

#include "multi.h"
void *multisender(void *args)
{
    list_arg *arg = (list_arg *)args;
    char *group = arg->group;
    int port = arg->port;

    unsigned int prio = 1;

    // create what looks like an ordinary UDP socket
    //
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        perror("socket");
        return 1;
    }

    // set up destination address
    //
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(group);
    addr.sin_port = htons(port);

    mqd_t queue = mq_open(SINGLE_QUEUE_NAME, O_RDWR);
    if (queue < 0)
    {
        perror("queue");
        return 1;
    }
    char message[MSGBUFSIZE];
    while (1)
    {
        int ready = 0;
        do
        {
            ssize_t size = mq_receive(queue, message, MSGBUFSIZE, &prio);
            if (size < 0)
            {
                sleep(1);
            }
            else
            {
                ready = 1;
            }
        } while (ready == 0);
        char ch = 0;
        int nbytes = sendto(
            fd,
            message,
            strlen(message),
            0,
            (struct sockaddr *)&addr,
            sizeof(addr));
        if (nbytes < 0)
        {
            perror("sendto");
            return 1;
        }
    }

    return 0;
}