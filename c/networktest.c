#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define SERVER_UDP_PORT "8500"
#define CLIENT_UDP_PORT "8501"

#define MAXLEN 1024

// calculate the time diff between start and end
long delay(struct timeval t1, struct timeval t2)
{
    long d;
    d = (t2.tv_sec - t1.tv_sec) * 1000000;
    d += t2.tv_usec - t1.tv_usec;
    return(d);
}

struct sockaddr_in server;
struct sockaddr_in client;

static void* serverThread(void *arg) {
    char buf[MAXLEN];

    int sd;

    /* create the UDP datagram socket */
    if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Can't create server socket\n");
        exit(1);
    }

    if(bind(sd,&server,sizeof(server))==-1) {
        perror("server bind error");
        exit(1);
    }

    socklen_t addrlen = sizeof(struct sockaddr_in);

    while(1) {
        if(recv(sd,buf,1024,0)==-1) {
            perror("recv on server error");
            exit(1);
        }
        if (sendto(sd, buf, MAXLEN, 0, (struct sockaddr *)&client, addrlen) == -1) {
            perror("sendto on server error");
            exit(1);
        }
    }

    return NULL;
}

int main(int argc, char **argv)
{
    int sd;
    char buf[MAXLEN];
    struct timeval start, end;

    server.sin_family = AF_INET;
    inet_pton(AF_INET,"localhost",&server.sin_addr);
    server.sin_port = SERVER_UDP_PORT;

    client.sin_family = AF_INET;
    inet_pton(AF_INET,"localhost",&client.sin_addr);
    client.sin_port = CLIENT_UDP_PORT;

    pthread_t tid;
    pthread_create(&tid,NULL,&serverThread,NULL);

    /* create the UDP datagram socket */
    if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Can't create client socket\n");
        exit(1);
    }

    if(bind(sd,&client,sizeof(client))==-1) {
        perror("client bind error");
        exit(1);
    }

    sleep(1); // let server thread be ready

    fprintf(stdout,"starting ping ping\n");

    gettimeofday(&start, NULL); /* start delay measurement */

    int i;

    for(i=0;i<50000;i++){
        socklen_t server_len = sizeof(struct sockaddr_in);
        if (sendto(sd, buf, MAXLEN, 0, (struct sockaddr *)&server, server_len) == -1) {
            perror("sendto on client error");
            exit(1);
        }
        if(recv(sd,buf,1024,0)==-1) {
            perror("recv on client error");
            exit(1);
        }
    }

    gettimeofday(&end, NULL); /* end delay measurement */

    printf("avg round-trip time = %ld us\n", delay(start, end)/50000);

    close(sd);
    return(0);
}
