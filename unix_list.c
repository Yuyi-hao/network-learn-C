// Listing local network addresses

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/socket.h> // system socket library 
#include<netdb.h> 
#include<ifaddrs.h>

int main(){
  struct ifaddrs *addresses; // stroes the addresses 
  if(getifaddrs(&addresses) == -1){
    // getifaddrs() fills the buffer of address as linked list return 0 if successfully ran else -1
    fprintf(stderr, "ERROR: getifaddrs call failed : %s\n", errno);
    exit(1);
  }
  struct ifaddrs *address = addresses; // address pointer to traverse linked list 
  while(address){
    int family = address->ifa_addr->sa_family; // get famliy of current address 
    if(family == AF_INET || family == AF_INET6){ // print only if its IPV$ or IPV6
      printf("%s\t", address->ifa_name); //  structure is defined in the ifaddrs.h header file and is used to represent information about a network interface on the system.
      printf("%s\t", family == AF_INET ? "IPV4" : "IPV6"); // print IPV4 or IPV6 
      char ap[100]; // to store textual address 
      const int family_size = family == AF_INET ?
	sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6); // get size of family 
      getnameinfo(address->ifa_addr,
		  family_size, ap, sizeof(ap), 0, 0, NI_NUMERICHOST);
      printf("\t %s \n", ap);
    }
    address = address->ifa_next;
  }
  freeifaddrs(address);
  
  return 0;
}
