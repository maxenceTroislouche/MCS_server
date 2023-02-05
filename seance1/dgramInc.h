#define CHECK(sts,msg) if ((sts) == -1) {perror(msg);exit(-1);}
#define PAUSE(msg) printf(msg); getchar();

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT_SVC 5000
#define INADDR_SVC "10.1.0.24"

#define MSG "Hello World!"
#define MAX_BUFF 1024
char buffer[MAX_BUFF];
