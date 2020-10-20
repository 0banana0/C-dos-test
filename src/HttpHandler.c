#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <netdb.h>
#include <errno.h>
#include "../inc/socketHelper.h"


static int socket_desc;
static struct sockaddr_in server;
static struct hostent *hp;
static char ip[20] = {0};

static void HTTPHandler()
{
    struct timeval timeout = {3, 0};

    puts("Connected\n");

    setsockopt(socket_desc, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));
}

int initSocket(char *web){
    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM , 0);
    if (socket_desc == -1){
        printf("Could not create socket");
        return -1;
    }

    if ((hp = gethostbyname(web)) == NULL){
        printf("Get hp failed");
        return -1;
    }

    strcpy(ip, inet_ntoa(*(struct in_addr *)hp->h_addr_list[0]));

    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(80);

    HTTPHandler();

    //Connect to remote server
    if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("connect error： %s", errno);
        return -1;
    }

    return 0;
}

int sendData(char *data)
{
    //向服务器发送数据
    if (send(socket_desc, data, strlen(data) , 0) < 0) {
        puts("Send failed\n");
        return 1;
    }

    printf("Data Send %s\n", data);
    
    return 0;
}

int receiveData(char *recBuf)
{
    //Receive a reply from the server
    //loop
    int size_recv, total_size = 0;
    while(1) {
        memset(recBuf , 0 , 512); //clear the variable
        //获取数据
        if ((size_recv =  recv(socket_desc, recBuf, 512, 0) ) == -1) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                printf("recv timeout ...\n");
                break;
            } else if (errno == EINTR) {
                printf("interrupt by signal...\n");
                continue;
            } else if (errno == ENOENT) {
                printf("recv RST segement...\n");
                break;
            } else {
                printf("unknown error: %d\n", errno);
                exit(1);
            }
        } else if (size_recv == 0) {
            printf("peer closed ...\n");
            break;
        } else {
            total_size += size_recv;
            printf("%s" , recBuf);
        }
    }

    printf("Reply received, total_size = %d bytes\n", total_size);
}