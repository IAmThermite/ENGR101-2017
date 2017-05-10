//Going off the 101 lecture from Arthur, 8/5/17
//10.140.30.163 is our ip of our pi

#include <stdio.h>
#include <time.h>
#include "E101.h"

#define THRESHOLD = 80;
#define GATE_DIST = 0;
const char *IP = "192.168.0.0"; //stores the IP of the server
const char *PASSWORD = "123456"; //password for the

extern "C" int init(int d_lev);
extern "C" int connect_to_server( char server_addr[15],int port);
extern "C" int send_to_server(char message[24]);
extern "C" int receive_from_server(char message[24]);

int quadrant = 1; //stores the number of the current quadrant


/**
 * will use the camera to try and find the line
 * will call the move method
 */ 
void find_line(){
	take_picture();
	char pix[320];
	
	int err = 0;
	int nwp = 0;//Number of white pixels
	
	for (int i=0; i<320; i++){
		pix[i] = get.pixel(120, i, 3);
		if (pix[i] > THRESHOLD){
			pix[i] = 1;
		} else {
			pix[i] = 0;
		}
	}
	for (int i=0; i<320; i++){
		if (pix[i] == 1){
			err = err + (i-160);
			nwp++;
		}
		
		if (nwp != 0){
			err = err/nwp;
			move(err);
		} else {
			back();
		}
	}
}

/**
 * moves the robot depending on the distance
 * from the line
 */
void move(int err){
	//Move towards the white line
	int speedLeft;
	int speedRight;
	double sc; //scale
	sc = 1; //for now, havent implemented it yet
	
	speedLeft = 80 + (int)((double)err*sc);//+ and - might be different depending on how the motors are connected
	speedRight = 80 - (int)((double)err*sc);
	
	set_motor(1, speedLeft);//might actually be right motor, need to test
	set_motor(2, speedRight);
	sleep1(0, 100000);
}

/**
 * Returns true if the gate has been opened
 */
bool open_gate() {
	connect_to_server("192.168.1.2", 1024);
	send_to_server(PASSWORD);
	
	char message[24]; //message from server
   	receive_from_server(message); //this may be buggy!
   	printf("From Server: %s\n", message);
   	
   	if(message == "") { //we have
		sleep1(2, 0); //make sure the gate has opened 
		printf("GATE OPENED\n");
		
		return true;
	} else {
		printf("ERROR, GATE NOT OPEN\n");	
		
		return false;
	}
}

/**
 * Moves the robot back a bit when we cant find the line
 */
void back(){
	//Error correcting by moving backwards if the whiteline cannot be found until one is found
	set_motor(1, 128);
	set_motor(2, -128);
	sleep1(0, 500000);
}

//MAIN QUADRANT METHODS

/**
 * The quadrant 1 code
 */
void quadrant1() {
	if(distance_to_gate() < GATE_DIST) {
		if(open_gate()) { //try to open the gate
			quadrant = 2;
		}
    } else {
		find_line();
	}
}

/**
 * The quadrant 2 code 
 */
 void quadrant2() {
	 
}

/**
 * Main method
 */
int main(){
	printf("My process ID : %d\n\n", getpid()); //for if we need to stop the process
	
    init();
    sleep1(0, 1000); //sleep a bit
    
    while(true) {
		if(quadrant == 1) { //do quadrant 1
			printf("	STARTING QUAD1\n");
			quadrant1();
		} else if(quadrant == 2) {
			printf("	STARTING QUAD2\n");
			quadrant2();	
		}
	}
    	
    return 0;
}
