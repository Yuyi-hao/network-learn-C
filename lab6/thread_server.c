#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
//the function handling client in multithreading
void *todo_client(void *);
int main(int argc , char *argv[]) {
	int socket_server, socket_client, sizeaddr;
	struct sockaddr_in server_addr, client_addr;
	//socket creation
	socket_server = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_server==-1) {
		printf("Could not be created");
		exit(1);
	}
	puts("Socket created");
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(8888);
	if(bind(socket_server, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		perror("bind failed. Error");
		return 1;
	}
	puts("binding has been completed");
	//listen
	listen(socket_server, 3);
	//accept the incoming connection
	puts("waiting for incoming connections..");
	sizeaddr = sizeof(struct sockaddr_in);
	pthread_t thread_id;
	while((socket_client = accept(socket_server, (struct sockaddr *)&client_addr, (socklen_t*)& sizeaddr))) {
		puts("connection accepted");
		if(pthread_create(&thread_id, NULL, todo_client, (void *)&socket_client)<0 ) {
			perror("thread could not be created");
			return 1;
		}
		//joining thread	
		pthread_join(thread_id, NULL);
		puts("Handler assigned");
	}
	if(socket_client<0) {
		perror("accept failed");
		return 1;
	}
	return 0;
}
void *todo_client(void *socket_descriptor)
{
    //get socket descriptor
    int socket_desc=*(int*)socket_descriptor;
    int read_size;
    char * message, client_message[2000];
    //send message
    message="greetings i ma your handler \n";
    write(socket_desc,message,strlen(message));
    message="type message\n";
    write(socket_desc,message,strlen(message));
    //receive message
    while((read_size=recv(socket_desc,client_message,2000,0))>0)
    {
        //end string marker
        client_message[read_size]='\0';
        //send message
        write(socket_desc,client_message,strlen(client_message));
        memset(client_message,0,2000);
    }
    if(read_size==0)
    {
        puts("clients disconnected");
        fflush(stdout);
    }
    if(read_size==-1)
    {
        perror("recv failed");
    }
    return 0;
}