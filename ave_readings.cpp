#include <stdio.h>
#include <time.h>
#include "E101.h"

int main() {
	init();
	int total = 0;
	int adc_reading = read_analog(0);
	
	for(int i = 0; i < 5; i++) {
		adc_reading = read_analog(0);
		total += adc_reading;
		
	    sleep1(1, 0);
	}
	printf("Average reading: %d\n", total/5);

	return 0;
}
