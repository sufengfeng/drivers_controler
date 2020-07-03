/*
 * threadt.cpp
 *
 *  Created on: Jul 3, 2020
 *      Author: yf
 */

#include "threadt.h"
#include "global.h"
#include "sensor.h"
#include "v4l2.h"
#include <pthread.h>
#include <thread>
#include <string>
#include <mutex>

#include "can_dev.h"

using namespace std;

//struct mutex mutex;
//mutex_init(&mutex); /*定义*/
//...
//mutex_lock(&mutex); /*获取互斥锁*/
//... /*临界资源*/
//mutex_unlock(&mutex); /*释放互斥锁*/



//传感器处理线程
void *SensorHanle(void*param){
	int iRet;
	while(1){
		if (g_GlobalParam.g_nRunStatus== STATE::DRV_STANDBY|| g_GlobalParam.g_nRunStatus== STATE::DRV_PAUSED) {
			mSleep(g_GlobalParam.g_nIdleSleepTime);                 //待机或者挂起
			continue;
		}
		iRet= GetDs18b20Value(g_GlobalParam.g_nDs18b20Value);
		if(iRet!=0)
			cout<<"GetDs18b20Value failed!!!"<<endl;
		iRet=GetLightValue(g_GlobalParam.g_nLightValue);
		if(iRet!=0)
			cout<<"GetLightValue failed!!!"<<endl;
		iRet=GetLimitValue(g_GlobalParam.g_nLimitValue);
		if(iRet!=0)
			cout<<"GetLimitValue failed!!!"<<endl;
		mSleep(g_GlobalParam.g_nPeriodSensor);
	}
	return (void*)0;
}

#define JPG_FILENAME "/tmp/current.jpg"			//当前保存图片的路径

//摄像头处理线程
void *CameraHanle(void*param){
	int iRet;
	while(1){
		if (g_GlobalParam.g_nRunStatus== STATE::DRV_STANDBY|| g_GlobalParam.g_nRunStatus== STATE::DRV_PAUSED) {
			mSleep(g_GlobalParam.g_nIdleSleepTime);                 //待机或者挂起
			continue;
		}
		iRet= SavePicture(JPG_FILENAME,g_GlobalParam.g_nExposure);
		if(iRet!=0)
				cout<<"SavePicture failed!!!"<<endl;
		mSleep(g_GlobalParam.g_nPeriodSensor);
	}
	return (void*)0;
}



//周期获取单次发送临时线程
void *cycleSend(void *param){
	int iRet;

	return (void*)0;
}
//单次触发单次发送临时线程
void *sigleSend(void *param){
	int iRet;

	return (void*)0;
}
/*
常规数据  id 0x1a   	6	1234	5		6
					len	ds18b20 Light	Limit
jpeg数据 id 0x1b 先发数据长度 0xfeeffddf12345678 接着发数据

接收		id 0x0a   		1		FF			EE DD
						len		command		param
command:
0x11	开机指令			获取当前状态
0x12	获取数据			获取缓存数据
0x13	工作指令			单次触发   暂时停止周期,单次采集,发送,打开周期
0x14	停机指令			停止工作等待命令

*/
//状态机线程
void *StatusHanle(void*param){
	int iRet=-1;
	int can_id;
	int len=0;
	char dataBuff[48];
	while(1){
		iRet =CanReceive(can_id,dataBuff,len);//阻塞等待
		if(iRet!=0)
			cout<<"CanReceive failed!!!"<<endl;
		if(len>0){
			switch(dataBuff[0]){
				case 0x11:		//开机指令			获取当前状态
					g_GlobalParam.g_nRunStatus= STATE::DRV_RUNNING;
					break;
				case 0x12:		//获取数据			发送缓存数据
				{
					pthread_t tmpProcess;
					pthread_create(&tmpProcess, NULL, cycleSend,NULL);
					pthread_detach(tmpProcess);
					break;
				};
				case 0x13:		//工作指令			设置曝光,立即生效 单次触发
				{
					pthread_t tmpProcess;
					pthread_create(&tmpProcess, NULL, sigleSend,NULL);
					pthread_detach(tmpProcess);
					break;
				}
				case 0x14:		//停机指令			停止工作等待命令
					g_GlobalParam.g_nRunStatus= STATE::DRV_STANDBY;
					break;
				default:cout<<"CanReceive failed!!!"<<dataBuff<<endl;
				break;

			}
		}else{
			cout<<"CanReceive failed!!!"<<endl;
		}
	}
	return (void*)0;
}

//初始化线程
int thread_init(void){
	pthread_t Process01;
	pthread_create(&Process01, NULL, SensorHanle, (void *)&g_GlobalParam);
	pthread_detach(Process01);

	pthread_t Process02;
	pthread_create(&Process02, NULL, CameraHanle, (void *)&g_GlobalParam);
	pthread_detach(Process02);

	pthread_t Process03;
	pthread_create(&Process03, NULL, StatusHanle, (void *)&g_GlobalParam);
	pthread_detach(Process03);

	return 0;
}
