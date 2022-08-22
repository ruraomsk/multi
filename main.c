#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for sleep()

#include <pthread.h>
#include <mqueue.h>

#include "multi.h"

int main(int argc, char *argv[])
{
    list_arg multi, single;
    pthread_t multi_listner, multi_sender, single_listner, single_sender;
    if (argc != 5)
    {
        printf("Command line args should be multicast group and port and unicast address and port\n");
        printf("(e.g., `multi 239.255.255.250 1900 192.168.115.201 1024`)\n");
        return 1;
    }

    multi.group = argv[1];      // e.g. 239.255.255.250 for SSDP
    multi.port = atoi(argv[2]); // 0 if error, which is an invalid port
    single.group = argv[3];
    single.port = atoi(argv[4]);
    if (pthread_create(&multi_listner, NULL, multilistner, (void *)&multi) != 0)
    {
        printf("Dont start multilistner\n");
        return 1;
    }
    if (pthread_create(&multi_sender, NULL, multisender, (void *)&multi) != 0)
    {
        printf("Dont start multisender\n");
        return 1;
    }
    if (pthread_create(&single_listner, NULL, udplistner, (void *)&single) != 0)
    {
        printf("Dont start udplistner\n");
        return 1;
    }
    if (pthread_create(&single_sender, NULL, udpsender, (void *)&single) != 0)
    {
        printf("Dont start udpsender\n");
        return 1;
    }
    while (1)
    {
        printf(".");
        sleep(10);
    }
}