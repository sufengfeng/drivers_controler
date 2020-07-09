# drivers_controler
can驱动控制模块研制开发
1、系统基于树莓派开发，使用魏雪电子的树莓派专用can模块，实现can数据的收发，经测试，can口发送速率最高速度可达到4s/帧。
2、温度传感器使用DS18b20，单次采集速率>1s/帧，需要进行优化
3、光电传感器和限位传感器基于树莓派IO库，高低点评判定
现存问题：
1、can模块发送速率过小，可能需要更改驱动
2、单次温度采集时间过久，需要更换温度采集方案
3、光电传感器只有在感应到物体才变化，需要优化检测到IO电平变化后，主动进行中断传输。
