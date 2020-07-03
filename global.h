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

std::string RndN(int N);
//状态机的状态
typedef enum {
	IDEL=0,
	KYLAND_CLOUD=0,
}STATUS_TYPE;

class GlobalParam {
public:
	GlobalParam();
	virtual ~GlobalParam();

public:
	uint16_t g_nBitrate;			//can波特率
	uint16_t g_nStatus;				//状态机


	vector<string> g_VarName;		//变量名称表
	string g_SysName="test2";		//edps默认工程名
	string g_HARDWARE_ID;			//设备唯一标示ID
	string g_Locations;					//设备位置信息
	bool g_UPmqttModbus=false;		//是否上传mqtt端modbus数据

//RDF model
	std::string	g_SN=RndN(32)	;//		设备出厂序列号	[1]
	std::string	g_name	="KGW";//		设备名称	[1]
	std::string	g_manufacturer	="kyland";//		设备生成厂商	[0,1]
	std::string	g_type	="TTU";//		设备类型，如“TTU”	[0,1]
	std::string	g_model	="KGW0.1";//		设备型号	[0,1]
	std::string	g_hardVer	="HV1.0";//		硬件版本	[0,1]
	std::string	g_softVer	="SV1.0";//		软件版本	[0,1]
	std::string	g_profileVer	="MV1.0";//		模型版本	[0,1]

	std::string	g_EquipmentID=RndN(32);//指向遥测关联的设备mRID[0,1]
	std::string	g_TerminalID=RndN(32);//指向遥测关联的拓扑端子mRID[0,1]
	//MQTT

	int g_mqtt_num=0;	//默认云端数量唯一
//	mqtt::MqttBasic * g_p_MqttBasic[10];	//最多支持10个mqtt客户端
	//ThREAD
	int g_MQTT_RUN=1;				//用来控制自身的运行
	bool flag_reload=true;
};


bool RecogniseSingle(void);
int string_format(string &str,const char* format, ...);
std::string string_format(const char* format, ...);


extern GlobalParam g_GlobalParam;		//全局配置参数



#define MARK_MQTT00 0xff00
#define MARK_MQTT01 0xff01

int GetDriverList(string &nameList );



#define FILELEN 128
typedef struct PACKED_CPU         //定义一个cpu occupy的结构体
{
char name[20];      //定义一个char类型的数组名name有20个元素
unsigned int user; //定义一个无符号的int类型的user
unsigned int nice; //定义一个无符号的int类型的nice
unsigned int system;//定义一个无符号的int类型的system
unsigned int idle; //定义一个无符号的int类型的idle
}CPU_OCCUPY;

typedef struct PACKED_MEM         //定义一个mem occupy的结构体
{
char name[20];      //定义一个char类型的数组名name有20个元素
unsigned long total;
char name2[20];
unsigned long free;
}MEM_OCCUPY;
int get_cpuoccupy (CPU_OCCUPY *cpust); //对无类型get函数含有一个形参结构体类弄的指针O
int get_memoccupy (MEM_OCCUPY *mem); //对无类型get函数含有一个形参结构体类弄的指针O
int cal_cpuoccupy (CPU_OCCUPY *o, CPU_OCCUPY *n);
int  init_devices(void );
#endif /* GLOBAL_H_ */
