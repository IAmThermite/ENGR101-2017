#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "E101.h"

int main() {
    init();
    
    char pix[320];
    int err = 0;
    
    while(true) {
        take_picture();
        for(int i = 0; i<320; i++) {
            pix[i] = get_pixel(120, i, 3);
            
            if(pix[i] > 80) {
                pix[i] = 1;
                err = err + (i-160);
            } else {
                pix[i] = 0;
            }
        }
        
        printf("%d\n", err);
        sleep1(0, 500000);
    }
    
    return 0;    
}
