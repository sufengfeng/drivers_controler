/*
 * Global.h
 *
 *  Created on: Jul 3, 2020
 *      Author: yf
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <iostream>
#include <ctime>//time.h
#include <vector>
#include <string>
#include <iostream>

#include <sys/utsname.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>
using namespace std;
#define psln(x)  LOG(ERROR) << #x " = " << (x)
//程序状态控制状态
typedef enum {
	DRV_STANDBY=0,
	DRV_PAUSED,
	DRV_RUNNING
}STATE;

//状态机的状态
typedef enum {
	IDEL=0,
	PRE_WORK,
	WORKING
}STATUS_TYPE;

class GlobalParam {
public:
	GlobalParam();
	virtual ~GlobalParam();

public:
	uint8_t g_nRunStatus=DRV_STANDBY;			//程序运行状态
	uint8_t g_nCurrentStatus=IDEL;						//状态机运行状态

	uint32_t g_nBitrate=1000000;				//can波特率

	uint16_t g_nIdleSleepTime=1000;				//空闲等待时间
	uint16_t g_nPeriodSensor=100;				//间隔100ms巡视传感器
	uint16_t g_nPeriodV4l2=1000;				//间隔1s读取一次图片
	uint32_t g_nExposure=1000;					//曝光时间

	int g_nLightValue=0;					//光电传感器值
	int g_nLimitValue=0;					//限位开关值
	int g_nDs18b20Value=0;					//温度计值

};
extern GlobalParam g_GlobalParam;		//全局配置参数
int  init_devices(void );
void mSleep(unsigned long milliseconds);
#endif /* GLOBAL_H_ */


