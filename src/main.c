#include <stdlib.h>
#include "../inc/socketHelper.h"

int main(int argc, char *argv[]){
    if(initSocket("www.cnblogs.com") == -1){
        exit(1);
    }
    HTTPHandler();

    return 0;
}