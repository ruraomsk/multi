// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <mqueue.h>

#include "multi.h"

void *singlesender(void *args)
{

    list_arg *arg = (list_arg *)args;
    char *group = arg->group;
    int port = arg->port;
    int sockfd;
    struct sockaddr_in servaddr;
    unsigned int prio = 1;
    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    int n, len;
    mqd_t queue = mq_open(MULTI_QUEUE_NAME, O_RDWR);
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
            sockfd,
            message,
            strlen(message),
            0,
            (struct sockaddr *)&servaddr,
            sizeof(servaddr));
        if (nbytes < 0)
        {
            perror("sendto");
            return 1;
        }
    }
}