#ifndef __CAN__H
#define __CAN__H

#include "global.h"
using namespace std;

int CanOpen(int bitrate);
int CanSend(int fd,int can_id,char *canData,int dataLen);
int CanReceive(int fd,int &can_id,char *canData,int &dataLen);
int CanClose(int fd);

#endif
