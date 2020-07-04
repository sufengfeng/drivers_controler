#ifndef __V4L2__H_
#define __V4L2__H_

#include "global.h"

using namespace std;

int V4l2Init(void);
int GetV4l2Status(void);
int SavePicture(string filenName,uint32_t exposure);
int v4l2_close(void);

#endif
