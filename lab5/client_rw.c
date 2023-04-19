#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<fcntl.h>

int main(int argc,char *argv[])
{
    //define server socket
    int clinet_socket;
    clinet_socket=socket(AF_INET,SOCK_STREAM,0);

    //define server address
    struct sockaddr_in server_address;
    server_address.sin_family=AF_INET;
    server_address.sin_addr.s_addr=INADDR_ANY;
    server_address.sin_port=htons(64000);

    int connection_status=connect(clinet_socket,(struct sockaddr*)&server_address,sizeof(server_address));
    if(connection_status==-1)
    {
        printf("\nerror in connection to server\n");
        exit(1);
    }

    char buff[256];

    read(clinet_socket,buff,sizeof(buff));
    printf("\n message received from server : %s\n",buff);
    char *msg;
    msg=(char *)malloc(sizeof(char)*50);
    printf("\nenter message for server : ");
    fgets(msg,50,stdin);
    write(clinet_socket,msg,strlen(msg));

    strcpy(buff,"");
    read(clinet_socket,buff,sizeof(buff));
    printf("\n message received from server : %s\n",buff);
    close(clinet_socket);
}