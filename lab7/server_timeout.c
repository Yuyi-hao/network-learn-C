#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
int main()
{
    int server_socket;
    server_socket=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in server_address;
    server_address.sin_family=AF_INET;
    server_address.sin_addr.s_addr=INADDR_ANY;
    server_address.sin_port=htons(64000);
    bind(server_socket, (struct sockaddr*)
    &server_address,sizeof(server_address));
    listen(server_socket, 10);
    printf("\nserver started listening");
    char* future[]={"You are hard-working.\n",
    "You will get a good job\n","You lose your friends\n",
    "Great carrer ahead\n","You will get good friends\n",
    "Today, you will get a surprise\n"};
    while(1)
    {
        int client_socket;
        client_socket=accept(server_socket,NULL,NULL);
        char* msg="Welcome, this is Aditya";
        write(client_socket, msg, strlen(msg));
        printf("\nmessage sent ...");
        char buff[256];
	do
	{
	
        read(client_socket,buff,sizeof(buff));
        printf("message received from client : %s \n",buff);
        char *p=strstr(buff,"END");
        if(buff[0]=='0')
        {
            printf("\nserver is going to off\n");
            break;
        }
        else
        {
            char *message=future[buff[0]-'1'];
            write(client_socket, message, strlen(message));
            printf("\nmessage sent ...");
        }
        }while(buff[0]!='0');
    }
    close(server_socket);
    return 0;
}