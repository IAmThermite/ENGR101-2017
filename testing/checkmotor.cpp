#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "E101.h"

int main() {
	init();
	printf("My process ID : %d\n\n", getpid()); //for if we need to stop the process
	sleep1(0, 10000);
	set_motor(1, 50);
	set_motor(2, 50);
	sleep1(2, 0);
	stop(1);
	stop(2);
	return 0;
}
