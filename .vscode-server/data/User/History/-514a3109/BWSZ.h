#define CHECK(sts,msg) if ((sts) == -1) {perror(msg);exit(-1);} 
#define PAUSE(msg) printf("%s\nAppuyez sur entree pour continuer\n",msg); getchar();
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>

#define PORT_SVC 5000
#define MSG "100"
#define OK "OK"
#define NOK "KO"
#define STOP "STOP"
#define MAX_BUFF 1024

char buffer[MAX_BUFF];
