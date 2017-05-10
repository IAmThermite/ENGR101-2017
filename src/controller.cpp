//Going off the 101 lecture from Arthur, 8/5/17
//10.140.30.163 is our ip

#include <stdio.h>
#include <time.h>
#include "E101.h"

#define THRESHOLD = 127;

int main(){
    init();
    sleep1(0, 1000);
    wherewhiteline();
    return 0;
}

int wherewhiteline(){
	//Find out where the white line is
	take_picture();
	char pix[320];
	int err = 0;
	int nwp = 0;//Number of white pixels
	for (int i=0; i<320; i++){
		pix[i] = get.pixel(120, i, 3);
		if (pix[i] > THRESHOLD){
			pix[i] = 1;
		}
		else {
			pix[i] = 0;
		}
		if (pix[i] == 1){
			err = err + (i-160);
			nwp++;
		}
		if (nwp != 0){
			err = err/nwp;
			move();
		}
		else {
			back();
		}
		return err;
	}
}

void move(){
	//Move towards the white line
	
}

void back(){
	//Error correcting by moving backwards if the whiteline cannot be found until one is found
	
}
