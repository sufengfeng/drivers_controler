#include "main.h"
#include "threadt.h"
#include "global.h"

void mainTest(void);

int main(void){
	//mainTest();
	init_devices();
	thread_init();
	while(1)sleep(100);	
    return 0;
}
//单元测试项
void mainTest(void){
	extern void canMainTest(void);
	canMainTest();
	// extern int v4l2MainTest(void);
	// v4l2MainTest();
	// extern int sensorMainTest(void);
	// sensorMainTest();
	exit(0);
}