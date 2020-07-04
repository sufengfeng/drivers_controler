#ifndef __SENSOR__H
#define __SENSOR__H
#include "global.h"
int SensorInit(void);
int GetDs18b20Value(int &retValue);
int GetLightValue(int &retValue);
int GetLimitValue(int &retValue);

#endif
