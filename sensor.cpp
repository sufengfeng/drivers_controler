
#include "sensor.h"
#include "global.h"
#include<stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <mutex>

mutex mtx_ds;
mutex mtx_Light;
mutex mtx_Limit;



//获得ds18b20数据
int GetDs18b20Value(int &retValue){
	mtx_ds.lock();

	mtx_ds.unlock();
    return  0;
}
//获得光照数据
int GetLightValue(int &retValue){
	mtx_Light.lock();

	mtx_Light.unlock();
    return 0;
}

//获得限位数据
int GetLimitValue(int &retValue){
	mtx_Limit.lock();

	mtx_Limit.unlock();
    return 0;
}
void sensorMainTest(void){
	int ds18b20_value,lightValue,limitValue;
	while(1){
		GetDs18b20Value(ds18b20_value);
		GetLightValue(lightValue);
		GetLimitValue(limitValue);
		printf("ds :%d light :%d limit :%d\n",ds18b20_value,lightValue,limitValue);


	}
}
