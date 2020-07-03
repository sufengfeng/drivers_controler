/*
 * threadt.cpp
 *
 *  Created on: Jul 3, 2020
 *      Author: yf
 */

#include "threadt.h"
#include "global.h"


#include <pthread.h>

//传感器处理线程
void *SensorHanle(void*param){


	return (void*)0;
}
//摄像头处理线程
void *CameraHanle(void*param){


	return (void*)0;
}

/*
常规数据  id 0x1a   	6	1234	5		6
					len	ds18b20 Light	Limit
jpeg数据 id 0x1b 先发数据长度 0xfeeffddf12345678 接着发数据

接收		id 0x0a   		1		FF			EE DD
						len		command		param
command:
0xf1	开机指令			获取当前状态
0xf2	获取数据			获取缓存数据
0xf3	工作指令			单次触发
0xf4	停机指令			停止工作等待命令

*/
//状态机线程
void *StatusHanle(void*param){


	return (void*)0;
}

//初始化线程
int thread_init(void){
	pthread_t palYunProcess01;
	pthread_create(&palYunProcess01, NULL, SensorHanle, (void *)&g_GlobalParam);
	pthread_detach(palYunProcess01);

	pthread_t palYunProcess02;
	pthread_create(&palYunProcess02, NULL, CameraHanle, (void *)&g_GlobalParam);
	pthread_detach(palYunProcess02);

	pthread_t palYunProcess03;
	pthread_create(&palYunProcess03, NULL, StatusHanle, (void *)&g_GlobalParam);
	pthread_detach(palYunProcess03);


	return 0;
}
