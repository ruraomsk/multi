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

void *singlelistner(void *args)
{

    list_arg *arg = (list_arg *)args;
    char *group = arg->group;
    int port = arg->port;
    unsigned int prio = 1;

    int sockfd;
    struct sockaddr_in servaddr, cliaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr,
             sizeof(servaddr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    int len, n;

    len = sizeof(cliaddr); // len is value/result
    mqd_t queue = mq_open(SINGLE_QUEUE_NAME, O_RDWR);
    if (queue < 0)
    {
        perror("queue");
        return 1;
    }
    char buffer[MSGBUFSIZE];
    while (1)
    {
        n = recvfrom(sockfd, (char *)buffer, MSGBUFSIZE,
                     MSG_WAITALL, (struct sockaddr *)&cliaddr,
                     &len);
        buffer[n] = '\0';
        int err = mq_send(queue, buffer, n, &prio);
        if (err < 1)
        {
            perror("send to queue");
            return 1;
        }
    }
}