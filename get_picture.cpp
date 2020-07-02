#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <linux/videodev2.h>

struct buffer
{
	void* start;
	unsigned int length;
};


int GetPictureMainTest(void)
{
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
 
	fp = fopen("/root/test.jpg", "wb+");
	if (fp < 0)
	{
		perror("fb open error.");
		return -1;
	}
 
	//打印支持的视频格式
	struct v4l2_fmtdesc fmtdesc;
	fmtdesc.index=0;
	fmtdesc.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
	printf("Support format:\n");
	while(ioctl(fd_video, VIDIOC_ENUM_FMT, &fmtdesc) != -1)
	{
		printf("\t%d.%s ,fmtdesc:%d\n",fmtdesc.index+1,fmtdesc.description,fmtdesc.pixelformat);
		fmtdesc.index++;
	}
	//设置视频的格式，720P JPEG
	struct v4l2_format s_fmt;
	s_fmt.fmt.pix.width = 1280;
	s_fmt.fmt.pix.height = 720;
	s_fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
	printf("s_fmt.fmt.pix.pixelformat:%d\n",s_fmt.fmt.pix.pixelformat);
	s_fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	int flag= ioctl(fd_video,VIDIOC_S_FMT,&s_fmt);
	if(flag != 0)
	{
		printf("set format error\n");
		return -1;
	}
 
	printf("jpeg:%d %d\n",s_fmt.fmt.pix.width,s_fmt.fmt.pix.height);
 
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
		exit (EXIT_FAILURE);
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
			exit(-1);
		buffers[n_buffers].length = buf.length;
		buffers[n_buffers].start = mmap (NULL,
				buf.length,PROT_READ | PROT_WRITE ,MAP_SHARED,fd_video, buf.m.offset);
		if (MAP_FAILED == buffers[n_buffers].start)
			exit(-1);
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
	printf("capture jpg finish..\n");
// system("eog -w /root/test.jpg &");
}
