#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <string>
#include <iostream>
using namespace std;

#include "global.h"
#include "can_dev.h"

int fd_can=-1;

int CanInit(void ){
    int can_fd=CanOpen(g_GlobalParam.g_nBitrate);
    if(can_fd<0){
        fprintf(stderr,"can send error!! \n");
        return -1;
    }
    return 0;
}

//初始化can0 并设置波特率
int CanOpen(int bitrate){

    system("sudo ifconfig can0 down");
    system("sudo ip link set can0 type can bitrate 1000000");
    system("sudo ifconfig can0 up");

    int ret;
    struct sockaddr_can addr;
    struct ifreq ifr;
    
    //1.Create socket
    fd_can = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (fd_can < 0) {
        perror("socket PF_CAN failed");
        return -1;
    }
    
    //2.Specify can0 device
    strcpy(ifr.ifr_name, "can0");
    ret = ioctl(fd_can, SIOCGIFINDEX, &ifr);
    if (ret < 0) {
        perror("ioctl failed");
        return -1;
    }
    
    //3.Bind the socket to can0
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    ret = bind(fd_can, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0) {
        perror("bind failed");
        return -1;
    }
    
    //4.Disable filtering rules, do not receive packets, only send
    //setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);

    return fd_can;
}
//循环发送字符串
int CanSend(int can_id,char *canData,uint32_t dataLen)
{
    int nbytes;
    struct can_frame frame;
    //Set send data
    frame.can_id =can_id;
    frame.can_dlc = dataLen;
    uint32_t sendCount=0;
    int errorCount=0;
   while (dataLen >sendCount)
   {
        if( dataLen-sendCount>8){
            frame.can_dlc=8;
        }else{
            frame.can_dlc=dataLen-sendCount;
        }
        //printf("sendCount  = 0x%X dlc = %d\r\n", sendCount,frame.can_dlc);
        for(int i = 0; i < frame.can_dlc; i++) 
            frame.data[i]=canData[sendCount+i];

        
        //for(int i = 0; i < frame.can_dlc; i++) printf("data[%d] = %d\r\n", i, frame.data[i]);
        //6.Send message
        nbytes = write(fd_can, &frame, sizeof(frame));
        if(nbytes != sizeof(frame)) {
            mSleep(5);
            errorCount++;
            fprintf(stderr,"can send error!! try again %d\n",errorCount);
            if(errorCount>10){
                fprintf(stderr,"try to send too much ,so return!! \n");
                return -1;
            }
            continue;
        }
        sendCount+=frame.can_dlc;
        mSleep(1);
   }
    return 0;
}
//阻塞接收数据
int CanReceive(int &can_id,char canData[],int &dataLen)
{
    struct can_frame frame;
    //5.Receive data and exit
    int nbytes=-1;
    nbytes = read(fd_can, &frame, sizeof(frame));
    if(nbytes <0) {    
        fprintf(stderr,"can receive error!! \n");
        return -1;
    }
    printf("can_id = 0x%X\r\ncan_dlc = %d \r\n", frame.can_id, frame.can_dlc);
    can_id=frame.can_id;
    dataLen=frame.can_dlc;
    for(int i = 0; i < frame.can_dlc; i++){
        canData[i]=frame.data[i];
        //printf("data[%d] = %d\r\n", i, canData[i]);
    } 
    return 0;
}

int CanClose(){
    close(fd_can);
    system("sudo ifconfig can0 down");
    return 0;
}
   
//测试函数

void canMainTest(void){
	int can_fd=CanOpen(g_GlobalParam.g_nBitrate);
	if(can_fd<0){
		fprintf(stderr,"can send error!! \n");
		return ;
	}

	char sendData[]={0xfe,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf,0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf,0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf,0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf,0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf,0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf};
    while(1){
        //sendData[0]++;
        std::cout << "/* message */" << std::endl;
	    CanSend(0x1a,sendData,sizeof(sendData));
        CanSend(0x1a,sendData,1);
        sleep(1);
    } 
    int iRet=-1;
	int can_id;
	int len=0;
	char dataBuff[48];
	while(1){
		iRet =CanReceive(can_id,dataBuff,len);//阻塞等待
		if(iRet!=0)
			cout<<"CanReceive failed!!!"<<endl;
        for(int i = 0; i < len; i++){
            printf("data[%d] = %d\r\n", i, dataBuff[i]);
        } 
    }
	CanClose();
}
