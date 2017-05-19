#include <stdio.h>
#include <time.h>
#include "E101.h"

void main() {
    init();
    sleep1(0, 1000);
    
    char pw[] = "123456789012345678901234";
    char ip[] = "130.195.6.196";
    char plz[] = "Please";
    char mts[7];
    int port = 1024;

    connect_to_server(ip, port);
	send_to_server(pw); //send a random string to server
	
	char message[24]; //message from server
   	receive_from_server(message); //this may be buggy!
   	printf("From Server: %s\n", message);
    
    for(int i = 0; i < 6; i++) {
        mts[i] = message[i];
    }
    
    mts[6] = 0;
    send_to_server(message);
    
    return 0;
}
