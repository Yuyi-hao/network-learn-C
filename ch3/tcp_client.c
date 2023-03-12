#include "ch3.h"

int main(int argc, char **argv){
  // initiate socket in windows
#if defined(_WIN32)
  WSADATA d;
  if(WSAStartup(MAKEWORD(2, 2), &d)){
    fprintf(stderr, "ERROR: Failed to initialize. \n");
    exit(1);
  }
#endif

  if(argc < 3){
    fprintf(stderr, "Usages: TCP client hostname and port name missing.\n");
    exit(1);
  }

  // get remote address
  printf("Configuring remote address ...\n");
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_socktype = SOCK_STREAM;
  struct addrinfo *peer_address;
  if(getaddrinfo(argv[1], argv[2], &hints, &peer_address)){
    fprintf(stderr, "ERROR: getinfoaddr() failed: %s\n", strerror(GETSOCKETERRNO()));
    exit(1);
  }

  // get address info 
  printf("Remote address is: ");
  char address_buffer[100];
  char service_buffer[100];
  getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen,
	      address_buffer, sizeof(address_buffer),
	      service_buffer, sizeof(service_buffer),
	      NI_NUMERICHOST);
  printf("%s %s\n", address_buffer, service_buffer);

  // create socket
  printf("Creating socket ......\n");
  SOCKET socket_peer;
  socket_peer = socket(peer_address->ai_family, peer_address->ai_socktype, peer_address->ai_protocol);
  if(!ISVALIDSOCKET(socket_peer)){
    fprintf(stderr, "ERROR: Scoket creating is failed : <%s>\n", strerror(GETSOCKETERRNO()));
    exit(1);
  }

  printf("Connecting... \n");
  if(connect(socket_peer, peer_address->ai_addr, peer_address->ai_addrlen)){
    fprintf(stderr, "ERROR: Connect() failed. <%s>\n", strerror(GETSOCKETERRNO()));
    exit(1);
  }
  freeaddrinfo(peer_address);

  printf("Connected.\n");
  printf("To send data, enter text(Press RET when you are done): ");

  while(1){
    fd_set reads;
    FD_ZERO(&reads);
    FD_SET(socket_peer, &reads);
#if !defined (_WIN32)
    FD_SET(0, &reads);
#endif
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 100000;
    if(select(socket_peer+1, &reads, 0, 0, &timeout) < 0){
      fprintf(stderr, "ERROR: Call to select()  has failed. : %s\n", strerror(GETSOCKETERRNO()));
      exit(1);
    }
    if(FD_ISSET(socket_peer, &reads)){
      char read[4*1024];
      int bytes_received = recv(socket_peer, read, 4*1024, 0);
      if(bytes_received < 1){
	printf("Connection closed by peer.\n");
	break;
      }
      printf("Received (%d bytes): %.*s", bytes_received, bytes_received, read);
    }
#if defined(_WIN32)
    if(_kbhit){
#else
    if(FD_ISSET(0, &reads)){
#endif
      char read[4*1024];
      if(!fgets(read, 4*1024, stdin)) break;
      printf("Sending: n%s", read);
      int bytes_sent = send(socket_peer, read, strlen(read), 0);
      printf("Sent %d  bytes.\n", bytes_sent);
    }	
  }
  printf("Closing socket...\n");
  CLOSESOCKET(socket_peer);
#if defined(_WIN32)
  WSACleanup();
#endif
  
  printf("Finished.\n");
  return 0;
}
