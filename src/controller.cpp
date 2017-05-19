//Going off the 101 lecture from Arthur, 8/5/17
//10.140.30.163 is our ip of our pi
//left motor = 1, right motor = 2
//g++ -o controler controller.cpp -I../ -le101

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "E101.h"

//constants
const int THRESHOLD = 80;
const int GATE_DIST = 0;
const int WALL_DIST = 0;

const char IP[] = "130.195.6.196"; //stores the IP of the server
const int PORT = 1024;
const char PASSWORD[] = "1234567890"; //password for the gate (can be anything)

const double SC_1 = 0.5; //error scale
const double SC_2 = 0.05; //derivitive scale

//things for networking
/*
extern "C" int init(int d_lev);
extern "C" int connect_to_server( char server_addr[15],int port);
extern "C" int send_to_server(char message[24]);
extern "C" int receive_from_server(char message[24]);
extern "C" int read_analog(int ch_adc); 
*/

int quadrant = 1; //stores the number of the current quadrant

int delta_err = 0; //the change in the error signal (err-prev_err)

//stores the pixels in the images
char pix1[320]; //first row of pixels
char pix2[320]; //second row of pixels
char pix_r[320];

/**
 * returns the ir reading for the distance to the gate
 */
int distance_to_wall() {
	int distance;
	distance = read_analog(0); //ir is at A0
	return distance;
}

/**
 * Checks to see if the line is completely white
 * in which case we are in quad3
 */
 bool is_full_white_line() {
	int nwp = 0; //Number of white pixels
	
	for (int i=0; i<320; i++){
		if (pix1[i] == 1){ //therefore white pixel
			nwp++;
		}
	}
    
    if(nwp > 270) { //may need to change this value
        return true;
    } else {
        return false;
    }
    
}

/**
 * Checks to see if the line is completely red
 */
 bool is_full_red_line() {
    int nrp = 0; //Number of red pixels
	
	for (int i=0; i<320; i++){
		if (pix_r[i] == 1){ //therefore red pixel
			nrp++;
		}
	}
    
    if(nrp > 270) { //may need to change this value
        return true;
    } else {
        return false;
    }
}

/**
 * will use the camera to try and find the line
 * will call the move method
 */
void find_line() {
    
    int err1 = 0; //error signal for first row
    int err2 = 0; //error signal for second row
	int nwp = 0; //Number of white pixels
	
	take_picture();
	for (int i=0; i<320; i++) {
        //first row of pixels
		pix1[i] = get_pixel(120, i, 3);
	
    	if (pix1[i] > THRESHOLD){ //therefore white pixel
			pix1[i] = 1;
            err1 = err1 + (i-160);
			nwp++;
		} else {
			pix1[i] = 0;
		}
        
        //second row of pixels
        pix2[i] = get_pixel(20, i, 3);
	
    	if (pix2[i] > THRESHOLD){ //therefore white pixel
			pix2[i] = 1;
            err2 = err2 + (i-160);
		} else {
			pix1[i] = 0;
		}
        
        pix_r[i] = get_pixel(120, i, 0); //get red chanel
        
        if(pix_r[i] > THRESHOLD) {
            pix_r[i] = 1;
        } else {
            pix_r[i] = 0;
        }
	}
    
    if (nwp != 0) { //if no white pixels
		err1 = err1/nwp;
		move(err1);
	} else {
		back(); //no white pixels found, we have lost the line
	}
    
    delta_err = err1 - err2;
}

/**
 * find line for the maze
 */
void find_line_maze() {
    
}

/**
 * Moves the robot back a bit when we cant find the line
 */
void back(){
	//Error correcting by moving backwards if the whiteline cannot be found until one is found
	set_motor(1, -50); //back so -ve left motor
	set_motor(2, 50);
	sleep1(0, 200000); //0.2 sec
}

/**
 * moves the robot depending on the distance
 * from the line
 */
void move(int err){
	//Move towards the white line
	int speed_left;
	int speed_right;
	
	speed_left = 80 - (int)((double)err*SC_1) - (int)((double)delta_err*SC_2);
	speed_right = 80 + (int)((double)err*SC_1) + (int)((double)delta_err*SC_2);
	
	set_motor(1, speed_left * -1); //left so must move in -ve direction 
	set_motor(2, speed_right); 
	sleep1(0, 50000); //50ms
}

/**
 * move depending on distance to wall
 */
void move() {
    int speed_left;
	int speed_right;
}

/**
 * Returns true if the gate has been opened
 */
void open_gate() {
	connect_to_server(IP, PORT);
	send_to_server(PASSWORD); //send a random string to server
	
	char message[24]; //message from server
   	receive_from_server(message); //this may be buggy!
   	printf("From Server: %s\n", message);
    
    send_to_server(message + "Please");
}


//
//MAIN QUADRANT METHODS
//

/**
 * The quadrant 1 code
 */
void quadrant1() {
    while(true) {
        if(distance_to_wall() < GATE_DIST) {
            open_gate(); //try to open the gate
            quadrant = 2;
            break;
            
        } else {
            find_line();
        }
    }
}

/**
 * The quadrant 2 code 
 */
 void quadrant2() {
     while(true) {
         if(!is_full_white_line()) {
             find_line();
        } else {
            quadrant = 3;
            break;
        }
    }
}

/**
 * The quadrant 3 code
 */
 void quadrant3() {
    while(true) {
        if(!is_full_red_line()) {
            find_line_maze();
        } else {
            quadrant = 4;
            break;
        }
    }
}

/**
 * The quadrant 4 code
 */
void quadrant4() {
    while(true) {
        
    }    
}

/**
 * Main method
 */
int main(){
	printf("My process ID : %d\n\n", getpid()); //for if we need to stop the process
	
    init();
    sleep1(0, 1000); //sleep a bit
    
    while(true) {
		if(quadrant == 1) {
			printf("	## STARTING QUAD1\n");
			quadrant1();
		} else if(quadrant == 2) {
			printf("	## STARTING QUAD2\n");
			quadrant2();	
		} else if(quadrant == 3) {
            printf("    ## STARTING QUAD2\n");
            quadrant3();   
        }
	}
    
    return 0;
}
