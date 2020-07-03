#include "main.h"
#include "threadt.h"
#include "global.h"

int main(void){
	init_devices();
	thread_init();
	while(1)sleep(100);
    return 0;
}

