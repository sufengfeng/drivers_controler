#include "stdio.h"
#include "can.h"

extern int GetPictureMainTest(void);
int main(void){
    int can_fd=CanOpen(1000000);
    if(can_fd<0){
        fprintf(stderr,"can send error!! \n");
        return -1;
    }
    
    char sendData[]="hello";
    CanSend(can_fd,0x1a,sendData,sizeof(sendData));
    CanClose(can_fd);

    GetPictureMainTest();

    return 0;
}
