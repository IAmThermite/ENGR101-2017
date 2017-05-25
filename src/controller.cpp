//Going off the 101 lecture from Arthur, 8/5/17
//10.140.30.163 is our ip of our pi
//left motor = 1, right motor = 2
//g++ -o controller controller.cpp -I../ -le101

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "E101.h"

//constants
const int THRESHOLD = 80;
const int RED_THRESHOLD = 200;
const int GATE_DIST = 300;
const int WALL_DIST = 0;

const double SC_1 = 0.23; //error scale
const double SC_2 = 0.001; //derivitive scale

int quadrant = 3; //stores the number of the current quadrant

int delta_err = 0; //the change in the error signal (err-prev_err)

//stores the pixels in the images
char pix1[320]; //first row of pixels
char pix2[320]; //second row of pixels
char pix_r[320];

/**
 * returns the ir reading for the distance to the gate
 */
int distance_to_wall() {
    int total = 0;
    
    for(int i = 0; i < 5; i++) {
	    total = total + read_analog(0); //ir is at A0
    }
	return (int)(total/5);
}


/**
 * moves the robot depending on the distance
 * from the line
 */
void move(int err){
	int speed_left;
	int speed_right;

	speed_left = 35 - (int)((double)err*SC_1) + (int)((double)delta_err*SC_2);
	speed_right = 35 + (int)((double)err*SC_1) - (int)((double)delta_err*SC_2);

    if(speed_left > 250) {
        speed_left = 250;
    } else if(speed_left < -250) {
        speed_left = -250;
    }

    if(speed_right > 250) {
        speed_right = 250;
    } else if(speed_right < -250) {
        speed_right = -250;
    }
	set_motor(1, speed_right);
	set_motor(2, speed_left * -1); //right so must move in -ve direction


    sleep1(0, 30000); //30ms
}


/**
 * Moves the robot back a bit when we cant find the line
 */
void back(){
	//Error correcting by moving backwards if the whiteline cannot be found until one is found
    printf("##BACK##\n");
	set_motor(1, -30);
	set_motor(2, 30);
	sleep1(0, 50000); //0.05 sec
}

/** 
 * Turns the robot left
 */
void turn_left(){
    printf("##LEFT##\n");
	set_motor(1,-40); 
	set_motor(2, -40);
    sleep1(2, 500000); //might need to adjust this
}

/** 
 * Turns the robot right
 */
void turn_right(){
	printf("##RIGHT##\n");
    set_motor(1, 40);
	set_motor(2, 40);
    sleep1(2, 500000); //might need to adjust this
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
        
        if(pix_r[i] > RED_THRESHOLD) {
            pix_r[i] = 1;
        } else {
            pix_r[i] = 0;
        }
	}
	delta_err = err1 - err2;
    
    //printf("DERR: %d\n", (int)delta_err*SC_2);

	if (nwp != 0) { //if no white pixels
		err1 = err1/nwp;
		move(err1);
	
    	//printf("NWP: %d, ERR: %d\n", nwp, err1);
	
    } else {
		back(); //no white pixels found, we have lost the line
	}
}

/**
 * find line for the maze (quadrant 3)
 */
void find_line_maze() {
	int nwp = 0;

    bool front_line = false;
	bool left_line = false;
	bool right_line = false;

	char pixF[320];
	char pixR[240];
	char pixL[240];

	while(true) {
        take_picture();

        //front
        for (int i=0; i<320; i++){
            pixF[i] = get_pixel(20, i, 3);

            if (pixF[i] > THRESHOLD){ //therefore white pixel
                pixF[i] = 1;
                nwp++;
            } else {
                pixF[i] = 0;
            }
        }
	printf("NWP: %d\n", nwp);
        if (nwp>20){
            front_line = true;
            break;
            
        }
        
        //left
        nwp = 0;
        for (int i=0; i<240; i++){
            pixL[i] = get_pixel(i, 20, 3);

            if (pixL[i] > THRESHOLD){ //therefore white pixel
                pixL[i] = 1;
                nwp++;
            } else {
                pixL[i] = 0;
            }
        }
	printf("NWP: %d\n", nwp);
        if (nwp>20){
            left_line = true;
            break;
        }


        //right
        nwp = 0;
        for (int i=0; i<240; i++){
            pixR[i] = get_pixel(i, 300, 3);

            if (pixR[i] > THRESHOLD){ //therefore white pixel
                pixR[i] = 1;
                nwp++;
            } else {
                pixR[i] = 0;
            }
        }
	printf("NWP: %d\n", nwp);
        if (nwp>20){
            right_line = true;
            break;
        }
        
        back(); //obvoiusly broken
    }
	
	
    if (front_line){
		printf("FRONT\n");
		find_line();
	} else if (left_line){
		turn_left();
		find_line();
	} else if (right_line){
		turn_right();
		find_line();
	}
	
}

/**
 * Trys to open the gate
 */
void open_gate() {
	char pw[] = "Please";
    char ip[] = "130.195.6.196";
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
    
    mts[6] = 0; //final digit is 0
    printf(mts);
    send_to_server(mts);
}


//
//MAIN QUADRANT METHODS
//

/**
 * The quadrant 1 code
 */
void quadrant1() {
    while(true) {
        if(distance_to_wall() > GATE_DIST) {
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
            printf("    ## STARTING QUAD3\n");
            quadrant3();   
        } else if(quadrant == 4) {
		printf("	## STARTING QUAD4\n");
	}

    }
    
    return 0;
}
