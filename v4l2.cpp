#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include "global.h"
#include <iostream>
#include <string>
#include "v4l2.h"
using namespace std;

struct buffer
{
	void* start;
	unsigned int length;
};

//初始化摄像头
int V4l2Init(void){
	return 0;
}
#include <mutex>

mutex mtx_V4l2;


	
//保存图片

/*
自动曝光: 	exposure  		0
手动曝光：	exposure 		1-9 
*/
int SavePicture(string fileName,uint32_t exposure){
	
	mtx_V4l2.lock();
	printf("Save %s, exposure: %d\n",fileName.c_str(),exposure);
	FILE* fp;
	int fd_video;
	struct buffer *buffers;
 
	//打开摄像头设备
	fd_video = open("/dev/video0", O_RDWR);
	if (fd_video < 0)
	{
		perror("video capture open");
		return fd_video;
	}
	fp = fopen(fileName.c_str(), "wb+");
	if (fp < 0)
	{
		perror("fb open error.");
		return -1;
	}
 
	//打印支持的视频格式
	// struct v4l2_fmtdesc fmtdesc;
	// fmtdesc.index=0;
	// fmtdesc.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
	// printf("Support format:\n");
	// while(ioctl(fd_video, VIDIOC_ENUM_FMT, &fmtdesc) != -1)
	// {
	// 	printf("\t%d.%s ,fmtdesc:%d\n",fmtdesc.index+1,fmtdesc.description,fmtdesc.pixelformat);
	// 	fmtdesc.index++;
	// }
#define PIC_W 640
#define PIC_H 480
// #define PIC_W 1280
// #define PIC_H 720
	//设置视频的格式，720P JPEG
	struct v4l2_format s_fmt;
	s_fmt.fmt.pix.width = PIC_W;
	s_fmt.fmt.pix.height = PIC_H;
	s_fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
	//printf("s_fmt.fmt.pix.pixelformat:%d\n",s_fmt.fmt.pix.pixelformat);
	s_fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	int flag= ioctl(fd_video,VIDIOC_S_FMT,&s_fmt);
	if(flag != 0)
	{
		printf("set format error\n");
		return -1;
	}

 	if (exposure!=0)		//default EXPOSURE_AUTO
	{
		// int ret; 
		// struct v4l2_control ctrl;
		// ctrl.id = V4L2_CID_EXPOSURE_AUTO;
		// ret = ioctl(fd_video, VIDIOC_G_CTRL, &ctrl);
		// if (ret < 0) 
		// {
		// 	printf("Get exposure auto Type failed\n");
		// 	return -1;
		// }
		// printf("\nGet Exposure Auto Type:[%d]\n", ctrl.value);

		// ctrl.id = V4L2_CID_EXPOSURE_AUTO; 
 		// ctrl.value = V4L2_EXPOSURE_MANUAL;  //手动曝光模式
 		// ret = ioctl(fd_video, VIDIOC_S_CTRL, &ctrl); 
		// if (ret < 0) 
		// { 
		// 	printf("Get exposure auto Type failed\n"); 
		// 	return -1; 
		// }
 		// //例2：在设置为手动模式后，再得到和设置曝光等级
		
		
		// ctrl.id = V4L2_CID_EXPOSURE;  //得到曝光档次，A20接受从 -4到4 共9个档次
		// ret = ioctl(fd_video, VIDIOC_G_CTRL, &ctrl); 
		// if (ret < 0) 
		// {
		// 	printf("Get exposure failed (%d)\n", ret); 
		// 	return -1; 
		// } 
		// printf("\nGet Exposure :[%d]\n", ctrl.value);

		// if(exposure<1)
		// 	exposure=1;
		// if(exposure>9)
		// 	exposure=9;
		// //设置曝光档次
		// ctrl.id = V4L2_CID_EXPOSURE; 
		// ctrl.value = exposure-5; 
		// ret = ioctl(fd_video, VIDIOC_S_CTRL, &ctrl); 
		// if (ret < 0) { 
		// 	printf("Set exposure failed (%d)\n", ret); 
		// 	return -1; 
		// }
	}
	//printf("jpeg:%d %d\n",s_fmt.fmt.pix.width,s_fmt.fmt.pix.height);
 
	//申请1个缓冲区
	struct v4l2_requestbuffers req;
	req.count=1;
	req.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory=V4L2_MEMORY_MMAP;
	ioctl(fd_video,VIDIOC_REQBUFS,&req);
	//缓冲区与应用程序关联
 
	//申请1个struct buffer空间
	buffers = (struct buffer*)calloc (req.count, sizeof (struct buffer));
	if (!buffers)
	{
		perror ("Out of memory");
		return -1;
	}
	unsigned int n_buffers;
	for (n_buffers = 0; n_buffers < req.count; n_buffers++)
	{
		struct v4l2_buffer buf;
		memset(&buf,0,sizeof(buf));
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = n_buffers;
		if (-1 == ioctl (fd_video, VIDIOC_QUERYBUF, &buf))
			return -1;
		buffers[n_buffers].length = buf.length;
		buffers[n_buffers].start = mmap (NULL,
				buf.length,PROT_READ | PROT_WRITE ,MAP_SHARED,fd_video, buf.m.offset);
		if (MAP_FAILED == buffers[n_buffers].start)
			return -1;
	}
 
	enum v4l2_buf_type type;
	for (n_buffers = 0; n_buffers < req.count; n_buffers++)
	{
		struct v4l2_buffer buf;
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = n_buffers;
		ioctl (fd_video, VIDIOC_QBUF, &buf);
	}
 
	//开始捕获图像
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ioctl (fd_video, VIDIOC_STREAMON, &type);
 
	struct v4l2_buffer buf;
	memset(&(buf), 0, sizeof(buf));
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	//取出图像数据
	ioctl (fd_video, VIDIOC_DQBUF, &buf);
	//保存图像
	fwrite(buffers[buf.index].start,1,buffers[buf.index].length,fp);//mjpeg
	fflush(fp);
	//放回缓冲区
	ioctl (fd_video,VIDIOC_QBUF,&buf);
 
	for (n_buffers = 0; n_buffers < req.count; n_buffers++)
		munmap(buffers[n_buffers].start, buffers[n_buffers].length);
	free(buffers);
 
	close(fd_video);
	fclose(fp);
	mtx_V4l2.unlock();
	return 0;
}

int GetV4l2Status(void){

	return 0;
}
//关闭摄像头
int v4l2_close(void){
	return 0;
}

int v4l2MainTest(void)
{
	SavePicture("/tmp/test2.jpg",1);
	return 0;
}


