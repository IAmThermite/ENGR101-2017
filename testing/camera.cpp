#include <stdio.h>
#include <time.h>
#include "E101.h"

int main(){
    init();
    sleep1(0, 1000);
    wherewhiteline();
    sleep1(5, 0);
    wherewhiteline();
    sleep1(5, 0);
    wherewhiteline();
    return 0;
}

void wherewhiteline(){
	take_picture();
	char pix[320];
	for (int i=0; i<320; i++){
		pix[i] = get.pixel(120, i, 3);
	}
	printf(pix);
	
}
