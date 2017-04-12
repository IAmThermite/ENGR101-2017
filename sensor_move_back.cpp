#include <stdio.h>
#include <time.h>
#include "E101.h"

int main() {
	init();
	
	int adc_reading = read_analog(0);
	while(true) {
		printf("Reading: %d\n", adc_reading);
		if(adc_reading > 100) { //move back
			set_motor(1, -128);
			set_motor(2, -128);
			sleep1(0, 500000);
		} else {
			set_motor(1, 0);
		}
		adc_reading = read_analog(0);
		sleep1(0, 5000);
	}
	
	return 0;
}
