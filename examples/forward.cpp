#include <stdio.h>
#include <time.h>
#include "E101.h"

int main() {
    init();
    sleep1(0, 1000);
    set_motor(1, 128);
    set_motor(2, -128);
    sleep1(1, 0);
    return 0;
}
