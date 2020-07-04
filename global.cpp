/*
 * Global.cpp
 *
 *  Created on: Jul 3, 2020
 *      Author: yf
 */


#include "global.h"
#include <iostream>
#include "can_dev.h"
#include "sensor.h"
#include "v4l2.h"

#define gettid() syscall(__NR_gettid) //获得线程ID

GlobalParam g_GlobalParam;		//全局配置参数
GlobalParam::GlobalParam() {
	// TODO Auto-generated constructor stub
}

GlobalParam::~GlobalParam() {
	// TODO Auto-generated destructor stub
}

//初始化硬件设备
int  init_devices(void ){
        int iRet=-1;
        iRet=CanInit();         //初始化can
        if (iRet<0)
        {
                fprintf(stderr,"CanInit error!! \n");
                return -1;
        }
	iRet=V4l2Init();            //初始化摄像头
        if (iRet<0)
        {
                fprintf(stderr,"V4l2Init error!! \n");
                return -1;
        }
        iRet=SensorInit();           //初始化传感器
        if (iRet<0)
        {
                fprintf(stderr,"SensorInit error!! \n");
                return -1;
        }
        fprintf(stderr,"Hardware Initial successfully!! \n");
	return 0;
}

#define __POSIX__
void mSleep(unsigned long milliseconds)
{
#if defined (_WIN32)
        Sleep(milliseconds);
#elif defined (__POSIX__)
        struct timespec reqTime, remTime;
        remTime.tv_sec = milliseconds / 1000;
        remTime.tv_nsec =  (milliseconds % 1000) * 1000000;
        int rv;
        do
        {
                reqTime = remTime;
                rv = nanosleep(&reqTime, &remTime);
        } while (-1 == rv && EINTR == errno);
#else // No native sleep functions are defined
#error Need define: _WIN32 or __POSIX__
#endif

}






