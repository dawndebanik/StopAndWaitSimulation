#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<unistd.h>
#include<string.h>
#include<semaphore.h>

#define RECV_PORT 8080
#define SEND_PORT 8090
