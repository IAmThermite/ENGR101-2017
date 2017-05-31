//10.140.30.163 is our ip of our pi
//left motor = 1, right motor = 2
//front IR = 0, left IR = 1
//g++ -o controller controller.cpp -I../ -le101

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "E101.h"

//constants
const int THRESHOLD = 80; //white value
const int RED_THRESHOLD = 120; //red value
const int GATE_DIST = 300; //distance to gate
const int WALL_DIST = 420; //front ir to wall close
const int WALL_DIST_2 = 200; //front ir to wall farther
const int WALL_DIST_3 = 500; //left ir to wall, should be as close to this value as possible

const double SC_1 = 0.12; //error scale
const double SC_2 = 0.03; //derivitive scale

const double SC_IR = 0.05; //error scale

int quadrant = 1; //stores the number of the current quadrant


//MISC METHODS
/**
 * Trys to open the gate
 */
void open_gate() {
	char pw[] = "Please High Ground";
    char ip[] = "130.195.6.196";
    char mts[7];
    int port = 1024;

    connect_to_server(ip, port);
	send_to_server(pw); //send Please to server
	
	char message[24]; //message from server
   	receive_from_server(message); //this may be buggy!
    
    for(int i = 0; i < 6; i++) {
        mts[i] = message[i];
    }
    
    mts[6] = 0; //final digit is 0
    printf(mts);
    send_to_server(mts);
}

/**
 * returns the number of white pixels on the row
 * takes the channel and row as inputs
 */
int get_num_pixels(int row, int channel) {
    int num = 0;
    char pix[320];
    
    take_picture();

    if(channel == 3) { //white
        for(int i = 0; i < 320; i++) {
            pix[i] = get_pixel(row, i, channel);
            if(pix[i] > THRESHOLD) {
                num++;
            }
        }
    } else { //not white so must be red
        for(int i = 0; i < 320; i++) {
            pix[i] = get_pixel(row, i, channel);
            if(pix[i] > RED_THRESHOLD) {
                num++;
            }
        }
    }

    return num;
}

/**
 * returns the number of white pixels on the column
 * takes the channel and column as inputs
 */
int get_num_pixels_col(int col, int channel) {
    int num = 0;
    
    take_picture();
    
    for(int i = 0; i < 240; i++) {
        if(get_pixel(i, col, channel) > THRESHOLD) {
            num++;
        }
    }
    
    printf("NUM: %d\n", num);
    return num;
}

/**
 * returns the number of error value of a line
 * takes the row as an input
 */
int get_error(int row) {
    int err = 0;
    int nwp = get_num_pixels(row, 3);
    char pix[320];
    
    take_picture();
    
    for(int i = 0; i < 320; i++) {
        pix[i] = get_pixel(120, i, 3);
        if(pix[i] > THRESHOLD) {
            err = err + (i-160);
        }
    }
    
    if(nwp <= 0) {
        return 0;
    } else {
        return (err/nwp);
    }
    
}


//IR METHODS
/**
 * returns the ir reading for the IR sensor at the specified pin
 */
int get_distance_to_wall(int pin) {
    int total = 0;
    
    for(int i = 0; i < 5; i++) {
	    total = total + read_analog(pin); //ir is at A0
    }
	return (int)(total/5);
}


//MOVE METHODS
/**
 * moves the robot depending on the distance
 * from the line
 */
void move(int err, int delta_err) {
	int speed_left;
	int speed_right;

	speed_left = 35 - (int)((double)err*SC_1) + (int)((double)delta_err*SC_2);
	speed_right = 35 + (int)((double)err*SC_1) - (int)((double)delta_err*SC_2);

    //ensure that the speed of the motors does not exceed 250/-250
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
    printf("err:%d derr:%d\n", (int)(err*SC_1), (int)(delta_err*SC_2));
    printf("L:%d R:%d\n", speed_left, speed_right);

	set_motor(1, speed_right);
	set_motor(2, speed_left * -1); //right so must move in -ve direction

    sleep1(0, 30000); //30ms
}

/**
 * Move method for the IR sensor
 */
void move_ir(int err) {
    
    int speed_left;
    int speed_right;
    
    speed_left = 50 - (int)((double)err*SC_IR);
    speed_right = 50 + (int)((double)err*SC_IR);
    
    //ensure that the speed of the motors does not exceed 250/-250
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

    printf("l: %d, r: %d\n", speed_left, speed_right);
    
	set_motor(1, speed_right);
	set_motor(2, speed_left * -1); //right so must move in -ve direction

    sleep1(0, 30000); //30ms
}

/**
 * Moves the robot back a bit when we cant find the line
 */
void back() {
	//Error correcting by moving backwards if the whiteline cannot be found
    printf("##BACK##\n");
	set_motor(1, -40);
	set_motor(2, 40);
	sleep1(0, 70000); //70ms
}

/** 
 * Turns the robot left
 */
void turn_left_line() {
    printf("##LEFT##\n");
    
    while(get_num_pixels(40, 3) < 10) { //while it cant find any pixels at front
	set_motor(1, -40);
	set_motor(2, -40);
        sleep1(0, 10000);
    }
}

/** 
 * Turns the robot right on the line
 */
void turn_right_line() {
    printf("##RIGHT##\n");
    
    while(get_num_pixels(40, 3) < 10) { //while it cant find any pixels at front
        set_motor(1, 40);
	    set_motor(2, 40);
        sleep1(0, 10000);
    }
}

/**
 * Turn left for the IR
 */
void turn_left_ir() { //turn left for 2.5 sec
    printf("##IR LEFT##\n");
    while(get_distance_to_wall(0) > WALL_DIST_2){
        set_motor(1, -40);
        set_motor(2, -40);
        sleep1(0, 50000);
    }

}

/**
 * Turn right for the IR
 */
void turn_right_ir() { //turn left for 2.5 sec
    printf("##IR RIGHT##\n");
    while(get_distance_to_wall(0) > WALL_DIST_2){
        set_motor(1, 40);
        set_motor(2, 40);
        sleep1(0, 50000);
    }
}


//LINE READING METHODS
/**
 * Checks to see if the line is completely white
 * in which case we are in quad3
 */
bool is_full_white_line() {
    if(get_num_pixels(120, 3) > 270) {
        return true;
    } else {
        return false;
    }
}

/**
 * Checks to see if the line is completely red
 */
bool is_full_red_line() {
    if(get_num_pixels(120, 0) > 270) {
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
    int err1 = get_error(120); //error signal for first row
    int err2 = get_error(20); //error signal for second row
	
    int nwp = get_num_pixels(120, 3); //Number of white pixels
	int delta_err = err1 - err2;
    
	if (nwp != 0) { //if no white pixels
		err1 = (int)(err1);
		move(err1, delta_err);
	
    } else {
		back(); //no white pixels found, we have lost the line
	}
}

/**
 * find line for the maze (quadrant 3)
 */
void find_line_maze() {
    //trigger a particular method
    int line = 0; //front 0, right 1, left 2

	while(true) {
        //front
        if (get_num_pixels(120, 3)>20){ //enough white for front line
            line = 0;
            break;
        }
        
        //left        
        if (get_num_pixels_col(20, 3)>20){ //enough white for left line
            line = 1;
            break;
        }

        //right
        if (get_num_pixels_col(300, 3)>20){
            line = 2;
            break;
        }
        
        back(); //obvoiusly broken
    }
	
    if (line == 0){
		printf("FRONT\n");
		find_line();
	} else if (line == 1){
		turn_left_line();
		find_line();
	} else if (line == 2){
		turn_right_line();
		find_line();
	}
}

/**
 * find the wall, for the IRWALL_DIST_3
 */
void find_wall() {
    int err_front = get_distance_to_wall(0);
    int err_side = get_distance_to_wall(1);
    int err = err_side - WALL_DIST_3;
    
    if(err_front < WALL_DIST) { //not too close at front
        if(err_side > WALL_DIST_3) { // too close to left so shuffle a bit
            move_ir(err * -1); //turn right
            
        } else { //too close right so shuffle a bit
            move_ir(err); //turn left
        }
        
    } else { //too close at front so we need to turn 90
        if(err_side > WALL_DIST_3) { //too close left
            turn_right_ir();
        } else {
            turn_left_ir();
        }
    }
}


//
//MAIN QUADRANT METHODS
//
/**
 * The quadrant 1 code
 */
void quadrant1() {
    while(true) {
        if(get_distance_to_wall(0) > GATE_DIST) {
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
        find_wall();
    }
}


//MAIN
/**
 * Main method
 */
int main(){	
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
            quadrant4();
        }
    }

    return 0;
}
