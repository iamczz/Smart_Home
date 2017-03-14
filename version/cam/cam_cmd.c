#include "cam.h"
#include "cam_cmd.h"
#include "image_process.h"

static int fd = -1; //yuyv 设备文件流
static struct buffer* yuyv_buffers0 = NULL ;
static unsigned int n_buffers = 0;
int numb = 0;
extern unsigned char buff[1024*80];

V4L2 v4l2_use = 
{
    .open = v4l2_lcd_init,
    .read = v4l2_capture_rgb,
    .write = lcd_display,
    .close = v4l2_lcd_close,
};

void v4l2_lcd_close(struct fb_lcd *lcdone)
{
    v4l2_close();
    lcd_close(lcdone);
}

int v4l2_lcd_init(void)
{
    char* v4l2_dev_name = DEVICE_VIDEO; //摄像头设备名
    char* lcd_dev_name = DEVICE_LCD; //LCD设备名
    unsigned char* all_rgb_buffers = (unsigned char*) malloc(WIDTH * HEIGHT * 3);//rgb 数据存储区

    v4l2_use.rgb_buffers = all_rgb_buffers;

    int flag = v4l2_open(v4l2_dev_name);//打开v4l2
    if(-1 ==flag)
	{
        printf("open video0 failed!\n");
        return -1;
    }
    fb_info.fbfd = lcd_init(lcd_dev_name,&fb_info);//打开LCD
    if(fb_info.fbfd < 0)
	{
        printf("open lcd failed!\n");
        return -1;
    }
	clear_screen(0, 1024, 0, 600, 0x00FFFFFF, &fb_info);//清屏
    surface_init(&fb_info);
}

int v4l2_open(char* dev_name)
{
    int ret = 0;

    struct v4l2_capability cap;
    struct v4l2_format fmt;
    unsigned int i = 0;
    enum v4l2_buf_type type;

    fd = open(dev_name, O_RDWR, 0 );//打开设备文件,阻塞模式
    if (fd < 0)
    {
        perror("open video0  fialed! ");
        return -1;
    }

    ret = ioctl(fd, VIDIOC_QUERYCAP, &cap);//查看设备功能
    if (ret < 0)
    {
        perror("requre VIDIOC_QUERYCAP fialed! \n");
        return -1;
    }
#if 0
    printf("driver:\t\t%s\n",cap.driver);
    printf("card:\t\t%s\n",cap.card);
    printf("bus_info:\t%s\n",cap.bus_info);
    printf("version:\t%d\n",cap.version);
    printf("capabilities:\t%x\n",cap.capabilities);
#endif
    if ((cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == V4L2_CAP_VIDEO_CAPTURE)
        printf("Device %s: supports capture.\n",dev_name);
    if ((cap.capabilities & V4L2_CAP_STREAMING) == V4L2_CAP_STREAMING)
        printf("Device %s: supports streaming.\n",dev_name);

    CLEAR(fmt);
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    fmt.fmt.pix.width = WIDTH;
    fmt.fmt.pix.height = HEIGHT;

    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;

    fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
    if(-1 == ioctl(fd, VIDIOC_S_FMT, &fmt))
	{
		//设置图片格式
        perror("set format failed!");
        return -1;
    }
    if(-1 == ioctl(fd, VIDIOC_G_FMT, &fmt)){//得到图片格式
        perror("set format failed!");
        return -1;
    }
#if 0
    printf("fmt.type:\t\t%d\n",fmt.type);
    printf("pix.pixelformat:\t%c%c%c%c\n", fmt.fmt.pix.pixelformat & 0xFF,(fmt.fmt.pix.pixelformat >> 8) & 0xFF,(fmt.fmt.pix.pixelformat >> 16) & 0xFF, (fmt.fmt.pix.pixelformat >> 24) & 0xFF);
    printf("pix.width:\t\t%d\n",fmt.fmt.pix.width);
    printf("pix.height:\t\t%d\n",fmt.fmt.pix.height);
    printf("pix.field:\t\t%d\n",fmt.fmt.pix.field);
#endif 
    struct v4l2_requestbuffers req;
    CLEAR(req);
    req.count = 5;//申请缓冲数量
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    ioctl(fd, VIDIOC_REQBUFS, &req);//申请缓冲，
    if (req.count < 2)
	{
        perror("buffer memory is Insufficient! \n");
        return -1;
    }

    yuyv_buffers0 = calloc(req.count, sizeof(*yuyv_buffers0));//内存中建立对应空间
    
    for (n_buffers = 0; n_buffers < req.count; ++n_buffers)
    {
        struct v4l2_buffer buf;//驱动中的一帧
        CLEAR(buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = n_buffers;

        if (-1 == ioctl(fd, VIDIOC_QUERYBUF, &buf))
        {
			//映射用户空间
			perror("VIDIOC_QUERYBUF error!\n");
			return -1;
        }

        yuyv_buffers0[n_buffers].length = buf.length;
        yuyv_buffers0[n_buffers].start =(unsigned char*) mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);

        if (MAP_FAILED == yuyv_buffers0[n_buffers].start)
        {
            close(fd);
            perror("mmap faild! \n");
            return -1;
        }
        //printf("Frame buffer %d: address = 0x%x, length = %d \n", n_buffers, (unsigned int)yuyv_buffers0[n_buffers].start,     yuyv_buffers0[n_buffers].length);
    }

    for (i=0; i<n_buffers; ++i)
    {
        struct v4l2_buffer buf;
        CLEAR(buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        if ( -1 == ioctl(fd, VIDIOC_QBUF, &buf) )
        {//申请到的缓冲进入队列
         close(fd);
         perror("VIDIOC_QBUF failed! \n");
         return -1;
        }
    }
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1 == ioctl(fd, VIDIOC_STREAMON, &type))
    {
		//开始捕捉图像数据
        close(fd);
        perror("VIDIOC_STREAMON failed!  ");
        return -1;
    }
    return 1;
}

int v4l2_capture_rgb(void)
{
    fd_set fds;
    struct timeval tv;
    int r;

    FD_ZERO(&fds);//将指定文件描述符集清空
    FD_SET(fd, &fds);//在文件描述符集合中增加一个新的文件描述符
    tv.tv_sec = 2;//time out
    tv.tv_usec = 0;
    r = select(fd+1, &fds, NULL, NULL, &tv);//判断摄像头是否准备好，tv是定时

    if(-1 == r)
	{
        if(EINTR == errno)
		{
            printf("select erro! \n");
        }
    }
    else if(0 == r)
	{
        printf("-------------select timeout! \n");//超时
        return 1;
    }
    read_frame(); //执行read_frame() 函数
    return 0;
}

int read_frame(void)
{
    struct v4l2_buffer buf;
    int ret =0;

    CLEAR(buf);
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    ret = ioctl(fd, VIDIOC_DQBUF, &buf);//出列采集的帧缓冲,成功返回0
    if(0 != ret)
	{
        printf("VIDIOC_DQBUF failed!\n");
        return -1;

    }
    yuyv_to_rgb(yuyv_buffers0[buf.index].start, v4l2_use.rgb_buffers);//yuyv -> rgb
    pthread_rwlock_wrlock(&cam_jpeg_mutex);
    v4l2_use.jpeg_size = rgb_to_jpeg(v4l2_use.rgb_buffers, (unsigned char *)v4l2_use.jpeg);
	pthread_rwlock_unlock(&cam_jpeg_mutex); 
   
    ret = ioctl(fd, VIDIOC_QBUF,&buf);//帧缓冲入列
    if(0 != ret){
        printf("VIDIOC_QBUF failed!\n");
        return -1;
    }
    return 1;
}

void v4l2_close(void)
{
    int i=0;
    for(i=0; i<n_buffers; ++i)
	{
        if(-1 == munmap(yuyv_buffers0[i].start, yuyv_buffers0[i].length))
		{
            printf("munmap error! \n");
            exit(-1);
        }
    }
    close(fd);
    free(v4l2_use.rgb_buffers);
    exit(EXIT_SUCCESS);
}
