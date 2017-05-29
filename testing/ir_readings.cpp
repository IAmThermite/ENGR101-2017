#include <stdio.h>
#include <time.h>
#include "E101.h"

void main() {
    init();
    sleep1(0, 1000);
    int total_ave;
    int total = 0;
    int readings = 10;
    int adc_reading;
    for(int i = 0; i < readings; i++) {
        for(int j = 0; j < readings; j++) {
            adc_reading = read_analog(0);
            
            sleep1(0, 100000);
            total = total+adc_reading;
        }
        printf("Ave of %d readings: %d\n", readings, total/readings);
    }
    
    printf("    Ave of %d average readings: %d\n", readings, total_ave/readings);
    
    for(int i = 0; i < readings; i++) {
        for(int j = 0; j < readings; j++) {
            adc_reading = read_analog(1);
            
            sleep1(0, 100000);
            total = total+adc_reading;
        }
        printf("Ave of %d readings: %d\n", readings, total/readings);
    }
    
    printf("    Ave of %d average readings: %d\n", readings, total_ave/readings);

}
