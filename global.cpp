/*
 * Global.cpp
 *
 *  Created on: Jul 3, 2020
 *      Author: yf
 */


#include "global.h"


GlobalParam g_GlobalParam;		//全局配置参数
GlobalParam::GlobalParam() {
	// TODO Auto-generated constructor stub
}

GlobalParam::~GlobalParam() {
	// TODO Auto-generated destructor stub
}
#include <iostream>

//用于初始化程序
class Initail{
public:
	Initail(){
		srand((int)time(NULL));  // 产生随机种子  把0换成NULL也行

		/**
		 * 判断是否时唯一进程，如果是，则开辟进程，并杀死父进程，进入守护进程
		 * 如果不是唯一进程，进入命令行模式
		 */

	}
};

Initail init;
#include "can_dev.h"
//初始化硬件设备
int  init_devices(void ){
	int can_fd=CanOpen(g_GlobalParam.g_nBitrate);
	if(can_fd<0){
		fprintf(stderr,"can send error!! \n");
		return -1;
	}
	int init_v4l2();
}

/**
 *产生N2
 */
std::string RndN(int N)
{
	std::string r = "";
    if(N<1)
    	return r;
    int i;

    char temp;
    for(i=0;i<N;i++)
    {
    	switch (rand()%2)
    	{
        case 0:
            temp= rand()%10 + '0';break;
        case 1:
            temp = rand()%26 + 'A';break;
//        case 2:
//            temp = rand()%26 + 'a';
//            break;
    	}
    	r+=temp;
    }
    return std::string(r);
}



#define TEST_PORT 63595
#define gettid() syscall(__NR_gettid) //获得线程ID





int get_memoccupy (MEM_OCCUPY *mem) //对无类型get函数含有一个形参结构体类弄的指针O
{
    FILE *fd;
    char buff[256];
    MEM_OCCUPY *m;
    m=mem;

    fd = fopen ("/proc/meminfo", "r");
    fgets (buff, sizeof(buff), fd);

    sscanf (buff, "%s %lu %s", m->name, &m->total, m->name2);
    fgets (buff, sizeof(buff), fd); //从fd文件中读取长度为buff的字符串再存到起始地址为buff这个空间里
    sscanf (buff, "%s %lu", m->name2, &m->free);

    fclose(fd);     //关闭文件fd
    return 0;
}

int cal_cpuoccupy (CPU_OCCUPY *o, CPU_OCCUPY *n)
{
    unsigned long od, nd;
    unsigned long id, sd;
    int cpu_use = 0;

    od = (unsigned long) (o->user + o->nice + o->system +o->idle);//第一次(用户+优先级+系统+空闲)的时间再赋给od
    nd = (unsigned long) (n->user + n->nice + n->system +n->idle);//第二次(用户+优先级+系统+空闲)的时间再赋给od

    id = (unsigned long) (n->user - o->user);    //用户第一次和第二次的时间之差再赋给id
    sd = (unsigned long) (n->system - o->system);//系统第一次和第二次的时间之差再赋给sd
    if((nd-od) != 0)
    cpu_use = (int)((sd+id)*10000)/(nd-od); //((用户+系统)乖100)除(第一次和第二次的时间差)再赋给g_cpu_used
    else cpu_use = 0;
    //printf("cpu: %u\n",cpu_use);
    return cpu_use;
}

int get_cpuoccupy (CPU_OCCUPY *cpust) //对无类型get函数含有一个形参结构体类弄的指针O
{
    FILE *fd;
    char buff[256];
    CPU_OCCUPY *cpu_occupy;
    cpu_occupy=cpust;
    fd = fopen ("/proc/stat", "r");
    fgets (buff, sizeof(buff), fd);
    sscanf (buff, "%s %u %u %u %u", cpu_occupy->name, &cpu_occupy->user, &cpu_occupy->nice,&cpu_occupy->system, &cpu_occupy->idle);
    fclose(fd);
    return 0;
}

int maintest()
{
    CPU_OCCUPY cpu_stat1;
    CPU_OCCUPY cpu_stat2;
    MEM_OCCUPY mem_stat;
    //获取内存
    get_memoccupy ((MEM_OCCUPY *)&mem_stat);

    //第一次获取cpu使用情况
    get_cpuoccupy((CPU_OCCUPY *)&cpu_stat1);
    sleep(10);

    //第二次获取cpu使用情况
    get_cpuoccupy((CPU_OCCUPY *)&cpu_stat2);

    //计算cpu使用率
    int cpu = cal_cpuoccupy ((CPU_OCCUPY *)&cpu_stat1, (CPU_OCCUPY *)&cpu_stat2);

    return 0;
}


