#include "lcd.h"
#include "zgb.h"

#define WIDTH 1024
#define HEIGTH 600
#define Bits_per_pixel 32

int lcd_init(char *device_name, struct fb_lcd *fb_info)
{
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;

    // Open the file for reading and writing
    fb_info->fbfd = open(device_name, O_RDWR);
    if(-1 == fb_info->fbfd)
    {
        perror("open");
        return -1;
    }
    printf("The framebuffer device was opened successfully.\n");

    // Get fixed screen information
    if (ioctl(fb_info->fbfd, FBIOGET_FSCREENINFO, &finfo)) 
	{
        printf("Error reading fixed information.\n");
        exit(2);
    }

    // Get variable screen information
    if (ioctl(fb_info->fbfd, FBIOGET_VSCREENINFO, &vinfo)) 
	{
        printf("Error reading variable information.\n");
        exit(3);
    }

    printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

    // Figure out the size of the screen in bytes
    fb_info->screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    // Map the device to memory
    fb_info->fbp = (char *)mmap(0, fb_info->screensize, PROT_READ | PROT_WRITE, MAP_SHARED,
            fb_info->fbfd, 0);
    if (fb_info->fbp == MAP_FAILED) 
	{
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");
    return 0;
}

//显示图片
void show_image(int x_wide, int y_high, const unsigned int image[], struct fb_lcd *fb_info)
{
    int p = 0;
    unsigned temp;
    long int location=(long int)fb_info->fbp+(WIDTH*4*60)+340*4;
    //printf("---%ld---",location);
    for(int y = 0; y < y_high; y++)
    {
        for(int x = 0; x < x_wide; x++)
        {
            //temp = image[p];
            temp = (((((image[p]&RGB565_MASK_RED)>>11)<<3)<<16)|((((image[p]&RGB565_MASK_GREEN)>>5)<<2)<<8)|
                    (((image[p]&RGB565_MASK_BLUE)<<3))|(0x00<<24));
            //printf("0x%x\n",temp);
            *(unsigned *)(location + x*4) = temp;
            p += 1;
        }
        location = location + WIDTH*4;
        //usleep(5000);
    }
}

void lcd_display(int x_wide, int y_high, unsigned char video[], struct fb_lcd *fb_info)
{
    int p = 0;
    unsigned temp;
    long int location=(long int)fb_info->fbp+(WIDTH*4*60)+340*4;
    //printf("---%ld---",location);
    for(int y = 0; y < y_high; y++)
    {
        for(int x = 0; x < x_wide; x++)
        {
	    temp = ((0x00<<24)|(video[p]<<16)|(video[p+1]<<8)|video[p+2]);
            *(unsigned *)(location + x*4) = temp;
            p += 3;
        }
        location = location + WIDTH*4;
    }
}


void display_pic(int x0,int x_wide,int y0, int y_high, unsigned char video[], struct fb_lcd *fb_info)
{
    int p = 0;
    unsigned temp;
    long int location=(long int)fb_info->fbp+(y0*4*1024)+x0*4;
    
    for(int y = 0;y < y_high ;y ++)
    {
        for(int x =0 ; x < x_wide ; x++ )
        {
			temp = ((0x00<<24)|(video[p]<<16)|(video[p+1]<<8)|video[p+2]);
            *(unsigned *)(location + x*4) = temp;
            p += 3;
        }
        location = location + 1024*4;
    }
}

#if 0
//将屏幕清屏
void clear_screen(int x0, int x_wide, int y0, int y_hight, unsigned color, struct fb_lcd *fb_info)
{
    for (int y = y0; y < y_hight; y++)
    {
        for (int x = x0; x < x_wide; x++) 
        {
            *(unsigned *)(fb_info->fbp + ((x + (y * x_wide))<<2)) = color;
        }
    }	
}
#endif
#if 1
void clear_screen(int x0, int x_wide, int y0, int y_hight, unsigned color, struct fb_lcd *fb_info)//清除一块特定的区域
{
	long int location=(long int)fb_info->fbp+x0*4+y0*1024*4;
	
	for(int i = 0; i < (y_hight); i++)
	{	
		memset((long int *)location, color,(x_wide)*4);
		location = location + 1024*4;//换行
	}
}
#endif
//画图片
void draw_picture( int x0, int x_wide, int y0, int y_hight, const unsigned int image[], struct fb_lcd *fb_info )
{
    int p = 0;
    unsigned int temp;
    for (int y = 0; y < y_hight; y++)
    {
        for (int x = 0; x < x_wide; x++) 
        {
            if((x + x0 < x_wide) && (y + y0 < y_hight))
            {   
                temp = (((((image[p]&RGB565_MASK_RED)>>11)<<3)<<16)|((((image[p]&RGB565_MASK_GREEN)>>5)<<2)<<8)|
                        (((image[p]&RGB565_MASK_BLUE)<<3))|(0x00<<24));
                *(unsigned *)(fb_info->fbp + ((x + (y * x_wide))<<2)) = temp;
            }
            p += 1;
        }
    }
}

//在屏幕中清除一块640*480区域
void clear_sector(int x_pos, int y_pos, int x_size, int y_size, struct fb_lcd *fb_info)
{
    long int location=(long int)fb_info->fbp+(WIDTH*y_pos*4)+(x_pos*4);
    for(int i = 0; i < y_size; i++)
    {	
        memset((long int *)location, 0x00FFFFFF,(WIDTH*4));
        location = location + WIDTH*4;
    }
}

//水平显示汉字
void show_H_words(int x_pos, int y_pos, const char incode[], struct fb_lcd *fb_info,unsigned color)
{
    unsigned char qh, wh;   
    unsigned long offset; 
    unsigned long location;
    for(int n = 0; incode[n] != 0; n++)
    {
        location = (WIDTH*y_pos*4)+(x_pos*4)+(48*4*n/2);
        //显示汉字
        int bytes  = 288;
        qh = incode[n]-0xa0;		//获得区码            
        wh = incode[n+1]-0xa0;		//获得位码               
        offset = (94*(qh-1)+(wh-1))*bytes;//得到HZK48中的偏移位置

        FILE *fp; 
        unsigned char buf[bytes];
        //GB2312字库文件hzk48
        if((fp=fopen("hzk48", "rb")) == NULL)  
        {   
            perror("fopen");   
            exit(0);   
        }
        //printf("fopen hzk48 successfully\r\n");   
        fseek(fp, offset, SEEK_SET);   
        int ret = fread(buf, bytes, 1, fp);
        if(ret < 0)
            exit(0);
        //printf("fread successfully\r\n");
        //显示
        int i = 0, j = 0;
        while(i < bytes){
            for(j = 0; j < 8; j++)
            {
                if(buf[i] & (0x80>>j))
                {
                    *((unsigned *)(fb_info->fbp + location)) = color;
                    location+=4;
                }
                else
                {
                    location+=4;
                }
            }

            if((++i%6) == 0)//扫描换行
            {
                location+=(WIDTH-48)*4;
                //printf("\r\n");
            }
        }
        n++;
        fclose(fp);
    }
}

//垂直显示汉字
void show_V_words(int x_pos, int y_pos, const char incode[], struct fb_lcd *fb_info, unsigned color)
{
    unsigned char qh, wh;   
    unsigned long offset; 
    unsigned long location = 0;
    
    location = (WIDTH*y_pos*4) + (x_pos*4);
    for(int n = 0; incode[n] != 0; n++)
    {
        //显示汉字
        int bytes  = 288;
        qh = incode[n]-0xa0;		        //获得区码            
        wh = incode[n+1]-0xa0;		        //获得位码               
        offset = (94*(qh-1)+(wh-1))*bytes;  //得到HZK48中的偏移位置

        FILE *fp; 
        unsigned char buf[bytes];
        //GB2312字库文件hzk48
        if((fp=fopen("hzk48", "rb")) == NULL)  
        {   
            perror("fopen");   
            exit(0);   
        }   
        fseek(fp, offset, SEEK_SET);   
        int ret = fread(buf, bytes, 1, fp);
        if(ret < 0)
            exit(0);
        //显示
        int i = 0, j = 0 ;
        while(i < bytes){//显示一个字
            for(j = 0; j < 8; j++)
            {
                if(buf[i] & (0x80>>j))
                {
                    *((unsigned *)(fb_info->fbp + location)) = color;
                    location+=4;
                }
                else
                {
                    location+=4;
                }
            }
            if((++i%6) == 0)//扫描换行
            {
                location+=(WIDTH-48)*4;
                //printf("\r\n");
            }
        }
        n++;
        fclose(fp);
    }
}

//显示字符串
void show_string(int x_pos, int y_pos, char *input, struct fb_lcd *fb_info, unsigned color)
{ 
    char incode[60];
    memset(incode, '\0', 60);
    strcpy(incode,input);

    int bytes  = 144;   
    unsigned long offset; 
    unsigned long location;
    for (int n = 0; incode[n] != 0; n++)
    {   
        location = ((WIDTH*4)*y_pos)+(x_pos*4)+(24*4*n);
        offset = (incode[n]-32)*bytes;//得到asc48中的偏移位置

        // 显示
        int bytes = 144;           
        offset = (incode[n]-32)*bytes;

        FILE *fp; 
        unsigned char buf[bytes];
        if((fp=fopen("ASC48", "rb")) == NULL)  
        {   
            perror("fopen");   
            exit(0);   
        }  
        fseek(fp, offset, SEEK_SET);   
        int ret = fread(buf, bytes, 1, fp);
        if(ret < 0)
            exit(0);

        int i = 0 , j = 0;
        while( i < bytes )
        {
            for(j = 0; j < 8; j++)
            {
                if(buf[i] & (0x80>>j))
                {
                    *((unsigned *)(fb_info->fbp + location)) = color;
                    location+=4;
                }
                else
                {
                    location+=4;
                }
            }
            if(++i%3==0)
            {
                location+=(WIDTH-24)*4;
            }
        }
        fclose(fp);
    }
}

void show_number(char number[], struct fb_lcd *fb_info, unsigned color)
{
    int bytes  = 144;   
    unsigned long offset; 

    unsigned long location = WIDTH*(HEIGTH-48)*4+(WIDTH-24)*4;      
    offset = (number[0]-32)*bytes;//得到asc48中的偏移位置

    FILE *fp; 
    unsigned char buf[bytes];
    if((fp=fopen("ASC48", "rb")) == NULL)  
    {   
        perror("fopen");   
        exit(0);   
    }  
    fseek(fp, offset, SEEK_SET);   
    int ret = fread(buf, bytes, 1, fp);
    if(0 > ret)
        exit(0);

    int i = 0 , j = 0;
    while( i < bytes )
    {
        for(j = 0; j < 8; j++)
        {
            if(buf[i] & (0x80>>j))
            {
                *((unsigned *)(fb_info->fbp + location)) = color;
                location+=4;
            }
            else
            {
                location+=4;
            }
        }
        if(0==++i%3)
        {
            location+=(WIDTH-24)*4;
        }
    }
    fclose(fp);
}

void lcd_close(struct fb_lcd *fb_info)
{
    if (-1==munmap(fb_info->fbp, fb_info->screensize))
        printf("munmap failed\n");
    printf("The framebuffer device was munmapped to memory successfully.\n");
    close(fb_info->fbfd);
    printf("The framebuffer device was closed successfully.\n");
    exit(0);
}

void surface_init(struct fb_lcd *fb_info)
{
	show_V_words(5, 10,"技术员联盟小组", fb_info, BLUE);//垂直显示汉字
	show_H_words(340, 8, "视频监控", fb_info, GOLDEN);    //水平显示汉字
	show_H_words(80, 400, "温度", fb_info, GREEN_YELLOW); //水平显示汉字
	show_string(170, 400, ":", fb_info, GREEN_YELLOW);
	show_H_words(230, 380, "。", fb_info, GREEN_YELLOW);
	show_string(250, 400, "C", fb_info, GREEN_YELLOW);
    show_H_words(80, 480, "湿度", fb_info, GREEN_YELLOW);  //水平显示汉字
	show_string(170, 480, ":", fb_info, GREEN_YELLOW);
	show_string(240, 480, "RH", fb_info,GREEN_YELLOW);
}