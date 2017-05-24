#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "E101.h"

int main() {
    
    int total = 0;
    
    for(int i = 0; i < 5; i++) {
	    total = total + read_analog(0); //ir is at A0
    }
    
    printf("ave: %d", (int)(total/5));
    
    return 0;    
}
