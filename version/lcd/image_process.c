#include "image_process.h"

#define WIDTH 640
#define HEIGTH 480

extern int numb;

long rgb_to_jpeg(unsigned char *rgb, unsigned char *jpeg)
{
	//printf("%s, %d\n", __func__,__LINE__);
    long unsigned jpeg_size;
    struct jpeg_compress_struct jcs;
    struct jpeg_error_mgr jem;
    JSAMPROW row_pointer[1];
    int row_stride;
    char *src=NULL;

    jcs.err = jpeg_std_error(&jem);
    jpeg_create_compress(&jcs);
    
    jpeg_mem_dest(&jcs, (unsigned char **)&src, &jpeg_size);

    jcs.image_width = WIDTH;
    jcs.image_height = HEIGTH;

    jcs.input_components = 3;
    jcs.in_color_space = JCS_RGB;//JCS_GRAYSCALE;

    jpeg_set_defaults(&jcs);
    jpeg_set_quality(&jcs, 80, TRUE);

    jpeg_start_compress(&jcs, TRUE);
    row_stride =jcs.image_width * 3;

    while(jcs.next_scanline < jcs.image_height) {//对每一行进行压缩
        row_pointer[0] = (JSAMPROW)(rgb + jcs.next_scanline * row_stride);
        (void)jpeg_write_scanlines(&jcs, row_pointer, 1);
    }
	
    jpeg_finish_compress(&jcs);
    memset(jpeg,0,jpeg_size);
    memcpy(jpeg,src,jpeg_size);
	free(src);
    jpeg_destroy_compress(&jcs);
    
#if 0   // 保存jpeg
    FILE *jpeg_fp;
    char path3[20];
    memset(path3, 0, 20);
    sprintf(path3, "jpeg%d.jpg", numb);
    if (numb == 100)
        numb = 0;
    jpeg_fp = fopen(path3,"w");
    if(jpeg_fp < 0 )
	{
        perror("open jpeg.jpg failed!\n");
        exit(-1);
    }
    fwrite(jpeg, jpeg_size, 1, jpeg_fp);
    fclose(jpeg_fp);
#endif 
    
    return jpeg_size;
}


void yuyv_to_rgb(unsigned char *yuv,unsigned char *rgb)
{
    unsigned int i;
    unsigned char y0,u0,y1,v0;

    float rt0 = 0, gt0 = 0, bt0 = 0, rt1 = 0, gt1 = 0, bt1 = 0;
//	printf("%s: %p, %p, x=%d, y=%d\n", __func__, yuv, rgb, X_SIZE, Y_SIZE);
    for(i = 0; i < ((X_SIZE * Y_SIZE) / 2); i++)
    {
        y0 = yuv[0];
        u0 = yuv[1];
        y1 = yuv[2];
        v0 = yuv[3];

        bt0 = 1.164 * (y0 - 16) + 2.018 * (u0 - 128); 
        gt0 = 1.164 * (y0 - 16) - 0.813 * (v0 - 128) - 0.394 * (u0 - 128); 
        rt0 = 1.164 * (y0 - 16) + 1.596 * (v0 - 128); 

        bt1 = 1.164 * (y1 - 16) + 2.018 * (u0 - 128); 
        gt1 = 1.164 * (y1 - 16) - 0.813 * (v0 - 128) - 0.394 * (u0 - 128); 
        rt1 = 1.164 * (y1 - 16) + 1.596 * (v0 - 128); 

        if(rt0 > 250)   rt0 = 255;
        if(rt0 < 0)     rt0 = 0;    

        if(gt0 > 250)   gt0 = 255;
        if(gt0 < 0)     gt0 = 0;    

        if(bt0 > 250)   bt0 = 255;
        if(bt0 < 0)     bt0 = 0;    

        if(rt1 > 250)   rt1 = 255;
        if(rt1 < 0)     rt1 = 0;    

        if(gt1 > 250)   gt1 = 255;
        if(gt1 < 0)     gt1 = 0;    

        if(bt1 > 250)   bt1 = 255;
        if(bt1 < 0)     bt1 = 0;    

        rgb[0] = (unsigned char)rt0;
        rgb[1] = (unsigned char)gt0;
        rgb[2] = (unsigned char)bt0;

        rgb[3] = (unsigned char)rt1;
        rgb[4] = (unsigned char)gt1;
        rgb[5] = (unsigned char)bt1;
       
        yuv += 4;
        rgb += 6;
    }
//	printf("%s:%d: %p, %p\n", __func__, __LINE__, yuv, rgb);
}

#if 0
int main()
{
    int yuyvfd, rgbfd;
    unsigned char *yuyvaddr, *rgbaddr;
    int ret = 0, i;
    unsigned char *jpeg;
    long jpeg_size;
    yuyvfd = open("out.yuv", O_RDONLY);
    if (yuyvfd < 0)
        perror("yuyvfd");

    rgbfd = open("rgb24.h", O_CREAT|O_RDWR);
    if (rgbfd < 0)
        perror("rgbfd");
       
    yuyvaddr = (char *)malloc(X_SIZE*Y_SIZE*2);
    if (yuyvaddr == NULL)
        printf("malloc error");
    rgbaddr = (char *)malloc(X_SIZE*Y_SIZE*3);
    if (rgbaddr == NULL)
        printf("malloc error");
    memset(rgbaddr, 0, (X_SIZE*Y_SIZE*3));

    ret = read(yuyvfd, yuyvaddr, (X_SIZE*Y_SIZE*2));
    if (ret < 0)
        printf("read failed");
    printf("read_ret=%d\n",ret);

    yuyv_to_rgb(yuyvaddr,rgbaddr);
    ret =  write(rgbfd, rgbaddr, (X_SIZE*Y_SIZE*3));
    if (ret < 0)
        printf("write failed");
    printf("write_ret=%d\n",ret);

    jpeg = (char *)malloc(X_SIZE*Y_SIZE*3);
    jpeg_size = rgb_to_jpeg(rgbaddr,jpeg);
    printf("%ld\n", jpeg_size);
    return  0;
}
#endif
