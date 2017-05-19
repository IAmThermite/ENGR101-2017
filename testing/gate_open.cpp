#include <stdio.h>
#include <time.h>
#include "E101.h"

void main() {
    init();
    sleep1(0, 1000);
    
    char pw[] = "123456789012345678901234";
    char ip[] = "130.195.6.196";
    char plz[] = "Please";
    int port = 1024;

    connect_to_server(ip, port);
	send_to_server(pw); //send a random string to server
	
	char message[24]; //message from server
   	receive_from_server(message); //this may be buggy!
   	printf("From Server: %s\n", message);
    
    send_to_server(message + plz);
    
    return 0;
}
