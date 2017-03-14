
#ifndef _IMAGE_PROCESS_H
#define _IMAGE_PROCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "jpeglib.h"

#define X_SIZE 640
#define Y_SIZE 480


long rgb_to_jpeg(unsigned char *rgb, unsigned char *jpeg);
void yuyv_to_rgb(unsigned char *yuv,unsigned char *rgb);

#endif
