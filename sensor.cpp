
#include "sensor.h"
#include "global.h"
#include<stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <mutex>

#include <fcntl.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#include<iostream>
#include<cstdlib>
#include<wiringPi.h>   

mutex mtx_ds;
mutex mtx_Light;
mutex mtx_Limit;

const int Lightpin = 0;
const int Limitpin = 4;
      
      
char w1_path[50] = "/sys/bus/w1/devices/";
int SensorInit(void){
    if(-1==wiringPiSetup())
    {
        cerr<<"setup error\n";
        return -1;
    }
    pinMode(Lightpin,INPUT);      
    pinMode(Limitpin,INPUT);      

	system("sudo modprobe w1-gpio");
    system("sudo modprobe w1-therm");
    char ID[20];
    DIR *dirp;
    struct dirent *direntp;
    int found = 0;
    if ( (dirp = opendir(w1_path)) == NULL)
    {   
        printf("open %s failure.\n", w1_path);  
        return -2; 
    }   

    while ( (direntp = readdir(dirp)) != NULL)
    {   
        if (strstr(direntp->d_name, "28-") != NULL)
        {   
            strcpy(ID, direntp->d_name);
            found = 1;
        }  
    }
    closedir(dirp);
    if (found != 1)
    {
        printf("can't find ds18b20.\n");
        return -3;
    }

    strcat(w1_path, ID);
    strcat(w1_path, "/w1_slave");
	return 0;
}
 
//获得ds18b20数据
int GetDs18b20Value(int &retValue){
	int iRet=-1;
	mtx_ds.lock();
	char buf[128];
    int fd = -1; 
    char *ptr;

	if ( (fd = open(w1_path, O_RDONLY)) < 0)
    {
        printf("open %s failure.\n", w1_path);
        iRet= -4;
		goto DS_SKIP;
    }
	if ( read(fd, buf, sizeof(buf)) < 0)
    {
        printf("read %s failure.", w1_path);
        iRet= -5;
		goto DS_SKIP;
    }
	close(fd);

    ptr = strstr(buf, "t=");
    if ( ptr == NULL)
    {
        printf("get temperature failure.\n");
        iRet= -6;
		goto DS_SKIP;
    }
    retValue = atoi(ptr + 2);
	printf("Tempera4ture is %d C\n", retValue);	
    mtx_ds.unlock();
    return  0;
DS_SKIP:    
	mtx_ds.unlock();
    return  iRet;
}
//获得光照数据
int GetLightValue(int &retValue){
	mtx_Light.lock();
    retValue=digitalRead(Lightpin);             
	mtx_Light.unlock();
    return 0;
}

//获得限位数据
int GetLimitValue(int &retValue){
	mtx_Limit.lock();
    retValue=digitalRead(Limitpin);
	mtx_Limit.unlock();
    return 0;
}
void sensorMainTest(void){
	int ds18b20_value=-1,lightValue=-1,limitValue=-1;
	SensorInit();
	while(1){
		GetDs18b20Value(ds18b20_value);
		GetLightValue(lightValue);
		GetLimitValue(limitValue);
		printf("ds :%d light :%d limit :%d\n",ds18b20_value,lightValue,limitValue);
		//sleep(1);
        delay(100);
	}
}




