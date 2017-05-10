//Going off the 101 lecture from Arthur, 8/5/17
//10.140.30.163 is our ip

#include <stdio.h>
#include <time.h>
#include "E101.h"

#define THRESHOLD = 80;
#define IP = ""; //stores the IP of the server
#define PASSWORD = "12345";
#define GATE_DIST = 0;

extern "C" int init(int d_lev);
extern "C" int connect_to_server( char server_addr[15],int port);
extern "C" int send_to_server(char message[24]);
extern "C" int receive_from_server(char message[24]);

int quadrant = 1; //stores the number of the current quadrant

int main(){
	printf("My process ID : %d\n", getpid()); //for if we need to stop the process
	
    init();
    sleep1(0, 1000); //sleep a bit
    
    while(true) {
		if(quadrant == 1) { //do quadrant 1
			quadrant1();
		}
	}
    	
    return 0;
}

/**
 * will calculate the error
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

void open_gate(char[24] password) {
	connect_to_server("192.168.1.2", 1024);
	send_to_server(password);
	
	char message[24];
   	receive_from_server(message); //this may be buggy!
   	printf("%s", message);
}

/**
 * Moves the robot back a bit
 */
void back(){
	//Error correcting by moving backwards if the whiteline cannot be found until one is found
	set_motor(1, 128);
	set_motor(2, -128);
	sleep1(0, 500000);
}

//MAIN QUADRANT METHODS
void quadrant1() {
	if(distance_to_gate() < GATE_DIST) {
		open_gate(PASSWORD);
	} else {
		find_line();
	}
}
