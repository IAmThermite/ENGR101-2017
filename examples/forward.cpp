#include <stdio.h>
#include <time.h>
#include "E101.h"

int main() {
    init();
	sleep1(0, 1000);
    set_motor(1, 254);
    set_motor(2, -254);
    sleep1(1, 0);
    return 0;
}
