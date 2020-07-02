#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>

//初始化can0 并设置波特率
int CanOpen(int bitrate){
    
    system("sudo ifconfig can0 down");
    system("sudo ip link set can0 type can bitrate 100000");
    system("sudo ifconfig can0 up");

    int ret;
    int s, nbytes;
    struct sockaddr_can addr;
    struct ifreq ifr;
    
    //1.Create socket
    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (s < 0) {
        perror("socket PF_CAN failed");
        return -1;
    }
    
    //2.Specify can0 device
    strcpy(ifr.ifr_name, "can0");
    ret = ioctl(s, SIOCGIFINDEX, &ifr);
    if (ret < 0) {
        perror("ioctl failed");
        return -1;
    }
    
    //3.Bind the socket to can0
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    ret = bind(s, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0) {
        perror("bind failed");
        return -1;
    }
    
    //4.Disable filtering rules, do not receive packets, only send
    //setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);
    return s;
}
//循环发送字符串
int CanSend(int fd,int can_id,char *canData,int dataLen)
{
    printf("can_id  = 0x%X can_dlc = %d\r\n", can_id,dataLen);
    int nbytes;
    struct can_frame frame;
    //Set send data
    frame.can_id =can_id;
    frame.can_dlc = dataLen;
    int sendCount=0;
   while (frame.can_dlc)
   {
       if( dataLen-sendCount>8){
            frame.can_dlc=8;
       }else{
            frame.can_dlc=dataLen-sendCount;
       }
       sendCount+=frame.can_dlc;
       memcpy(frame.data,canData+sendCount,frame.can_dlc);
        //for(int i = 0; i < frame.can_dlc; i++) printf("data[%d] = %d\r\n", i, frame.data[i]);
        //6.Send message
        nbytes = write(fd, &frame, sizeof(frame)); 
        if(nbytes != sizeof(frame)) {
            fprintf(stderr,"can send error!! \n");
            return -1;
        }
   }
    return 0;
}
//阻塞接收数据
int CanReceive(int fd,int &can_id,char *canData,int &dataLen)
{
    struct can_frame frame;
    //5.Receive data and exit
    int nbytes=-1;
    nbytes = read(fd, &frame, sizeof(frame));
    if(nbytes <0) {    
        fprintf(stderr,"can receive error!! \n");
        return -1;
    }
     printf("can_id = 0x%X\r\ncan_dlc = %d \r\n", frame.can_id, frame.can_dlc);
    can_id=frame.can_id;
    dataLen=frame.can_dlc;
    memcpy(canData,frame.data,frame.can_dlc);
    //for(int i = 0; i < 8; i++) printf("data[%d] = %d\r\n", i, frame.data[i]);
    return 0;
}

int CanClose(int fd){
    close(fd);
    system("sudo ifconfig can0 down");
}
   
