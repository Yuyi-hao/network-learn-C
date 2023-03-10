#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>


// custom defination
#define SOCKET int
#define ISVALIDSOCKET(s) ((s) >= 0)


int main(){
    printf("Configuring local address.....\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *bind_address;
    getaddrinfo(0, "8080", &hints, &bind_address);
    // create server socket
    printf("create socket.. \n");
    SOCKET socket_listen; // as windows has another type of socket and unix has it as int so
    socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
    // verify socket
    if(!ISVALIDSOCKET(socket_listen)){
      fprintf(stderr, "ERRROR: socket <%d> is invalid one: %s.\n", socket_listen, strerror(errno));
      exit(1);
    }

    printf("Binding sokcet to loacl addrress....\n");
    if(bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen)){
      fprintf(stderr, "ERROR: binding of socket failed: %s\n", strerror(errno));
      exit(1);
    }
    freeaddrinfo(bind_address);

    // listening for the new coonections
    printf("server is listening for new connections......\n");
    if(listen(socket_listen, 10) < 0){
      fprintf(stderr, "ERROR: can't listen. `%s`\n", strerror(errno));
      exit(1);
    }

    printf("Waiting for connection.....\n");
    struct sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);
    SOCKET socket_client = accept(socket_listen, (struct sockaddr*) &client_address, &client_len);
    if(!ISVALIDSOCKET(socket_client)){
      fprintf(stderr, "ERRROR: Could not connect to client accept() call failed: %s.\n", socket_client, strerror(errno));
      exit(1);
    }

    printf("Client is connected....\n");
    char address_buffer[100];
    getnameinfo(
		(struct sockaddr*) &client_address,  client_len, address_buffer, sizeof(address_buffer), 0, 0,  NI_NUMERICHOST);
    printf("buffer address: %s\n", address_buffer);

    // reading request from server ig
    printf("Reading request....\n");
    char request[1024];
    int bytes_recieved = recv(socket_client, request, 1024, 0);
    // send response
    printf("Sending reponse.....\n");
    const char *response =
      "HTTP/1.1 200 OK\r\n"
      "Connection: Close\r\n"
      "Content-Type: text/plain\r\n\r\n"
      "Local time is: ";
    int bytes_sent = send(socket_client, response, strlen(response), 0);
    printf("Sent %d of %d bytes. \n", bytes_sent, (int)strlen(response));
    time_t timer;
    time(&timer);
    char *time_msg = ctime(&timer);
    printf("Local time is: %s\n", ctime(&timer));
    bytes_sent = send(socket_client, time_msg, strlen(time_msg), 0);
    printf("Sent %d of %d bytes. \n", bytes_sent, (int)strlen(time_msg));
    printf("Closing connection......>\n");
    close(socket_client);
    printf("Finished->\n");
    return 0;
}
