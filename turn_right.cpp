#include <stdio.h>
#include <time.h>
#include "E101.h"

void turn_right(int duration) {
	sleep1(0, 1000);
	set_motor(1, 0);
	set_motor(2, 128); //left motor on
	sleep1(duration, 0); //spin for duration seconds
}


int main() {
	init();
	sleep1(0, 1000);
	turn_right(2);
	
	return 0;
}
