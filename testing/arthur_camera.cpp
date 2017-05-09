#include <stdio.h>
#include <time.h>
#include "E101.h"

double err;
int nwp;

void where_white_line() {
	take_picture();
	char pix[320];
	char wh[320];
	err = 0;
	nwp = 0;
	for(int i=0; i<320; i++) {
		pix[i] = get_pixel(120, i, 3);
		wh[i] = 0 ;
		if (pix[i]>90) {
			wh[i]= 1;
			err = err + (double)(i-160);
			nwp++;
		}
	}
	if ( nwp>0){
	  err = err/((double)nwp);
     }
	// DEBUG from here
	display_picture(0, 100000);
	for(int i=0; i<320; i++) {
       printf("%d ",pix[i]);
    }
    printf("\n");
   	for(int i=0; i<320; i++) {
       printf("%d ",wh[i]);
    }
    printf("\n");
    printf("err=%f nwp=%d \n",err,nwp);
    
	printf("	###Image###\n");
}

int main() {
    init();
    while(1)
    {
     where_white_line();
     sleep1(1,0);
    }
    return 0;
}
