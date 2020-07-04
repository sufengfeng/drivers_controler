#ifndef __CAN__H
#define __CAN__H

#include "global.h"
using namespace std;
int CanInit(void );
int CanOpen(int bitrate);
int CanSend(int can_id,char *canData,uint32_t dataLen);
int CanReceive(int &can_id,char canData[],int &dataLen);
int CanClose(void);

#endif
