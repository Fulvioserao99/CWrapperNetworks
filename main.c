#include <stdio.h>
#include <stdlib.h>

#include "functions.h"

int main()
{
    int socketfd, listenfd,pid;
    char buffer[4096];
    char ricezione[4096];
    char indirizzo[100];

    socketfd = Socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in server;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_family = AF_INET;
    server.sin_port = htons(52000);

    Bind(socketfd,(struct sockaddr*)&server,sizeof(server));
    Listen(socketfd,1024);




    for(;;){

        listenfd = Accept(socketfd,NULL,NULL);
        FullWrite(1,"tutto ok\n",10);
        if((pid = fork()) < 0){
            perror("fork");
            exit(-1);
        }
        if (!pid){
            close(socketfd);
            fd_set readset;
           while (1){

                puts("alive\n");
                FD_ZERO(&readset);
                FD_SET(STDIN_FILENO,&readset);
                FD_SET(listenfd,&readset);

                Select(listenfd+1,&readset,NULL,NULL,NULL);

                if(FD_ISSET(listenfd,&readset)){
                    ssize_t len = read(listenfd,buffer,sizeof(buffer));
                    write(listenfd,buffer,len);
                    if (len == 4){
                        close(listenfd);
                        exit(0);
                    }

                }

                if(FD_ISSET(STDIN_FILENO,&readset)){
                    ssize_t len = read(STDIN_FILENO,buffer,sizeof(buffer));
                    write(listenfd,buffer,len);

                }





           }

        }
        else{
            close(listenfd);
            wait();
            puts("Sta morendo il server\n");
            exit(0);
        }

    }

    return 0;
}
