#include <stdio.h>
#include <time.h>
#include "E101.h"

void turn_left(int duration) {
	sleep1(0, 1000);
	set_motor(2, 0);
	set_motor(1, 128); //right motor on
	sleep1(duration, 0); //spin for duration seconds
}


int main() {
	init();
	sleep1(0, 1000);
	turn_left(2);
	
	return 0;
}
