#include <stdlib.h>
#include "../inc/socketHelper.h"

#define MAXSIZE 1024

int main(int argc, char *argv[])
{
    char buff[MAXSIZE];

    if(initSocket("www.cnblogs.com") == -1){
        exit(1);
    }

    sendData("GET / HTTP/1.1\r\nHOST: www.cnblogs.com\r\n\r\n");
    receiveData(buff);

    return 0;
}