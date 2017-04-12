#include <stdio.h>
#include <time.h>
#include "E101.h"

void reverse(int duration) {
	sleep1(0, 1000);
	set_motor(2, -55);
	set_motor(1, -55);
	sleep1(duration, 0); //spin for duration seconds
}


int main() {
	init();
	sleep1(0, 1000);
	reverse(2);
	
	return 0;
}
