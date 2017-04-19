#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include "common.h"

void play(int sock){
    
    printf("Waiting for the game to begin....\n");
    while(1){
        int n;
        char buffer[BUFFER_SIZE];
        if (0 > (n = read(sock, buffer, BUFFER_SIZE)))
        {
            /* error("Error reading from client"); */
            printf("Response from socket  timed out\n");
            
        }
        else{
            printf("%s\n",buffer);
        }

         if (0 > (n = read(sock, buffer, BUFFER_SIZE)))
        {
            /* error("Error reading from client"); */
            printf("Response from socket  timed out\n");
            
        }
        else{
            printf("%s\n",buffer);
        }



        //Now take the input from the user        
        int d;
        
        while(1){
            scanf("%d",&d);
            if(d==1){

                int nwritten;
                strcpy(buffer,"HIT");
                printf("Sending...%s\n",buffer);
                if (BUFFER_SIZE != (nwritten = write(sock, buffer, BUFFER_SIZE)))
                        error("Error! Couldn't write to server");




            }
            else{
                strcpy(buffer,"STAND");
                int nwritten;
                printf("Sending....%s\n",buffer);
                if (BUFFER_SIZE != (nwritten = write(sock, buffer, BUFFER_SIZE)))
                        error("Error! Couldn't write to server");

                break;
            }

        }
         if (0 > (n = read(sock, buffer, BUFFER_SIZE)))
        {
            /* error("Error reading from client"); */
            printf("Response from socket  timed out\n");
            
        }
        else{
            printf("\n %s \n",buffer);
        }


         if (0 > (n = read(sock, buffer, BUFFER_SIZE)))
        {
            /* error("Error reading from client"); */
            printf("Response from socket  timed out\n");
            
        }
        else{
            printf("\n %s \n",buffer);
        }






        



    }
    //return;
}

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr; 

    if(argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    } 

    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000); 

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    } 

   else{
     char buffer[BUFFER_SIZE];
     int nwritten;
     printf("Connected to server -- \n");
     play(sockfd);
   }

    return 0;
}