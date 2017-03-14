/*****************************************
        
>      从套接字中指定读取n个字节,
>      成功：返回实际读到的字节数,
>      失败：返回-1
 
 **************************************** */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
                                            //size_t 是无符号整型数；
ssize_t readn(int fd,void *vptr,size_t n)   //ssize_t 是有符号整型数；
{
    size_t nleft;
    ssize_t nread;
    char *ptr;
    ptr = vptr;
    nleft = n;

	while(nleft > 0)
	{
		if((nread = read(fd, ptr, nleft)) < 0)
		{
			if(errno == EINTR)//此处为read函数被系统信号叫醒，并未读取到数据
				nread = 0;
			else
				return -1;
		}
		else if(nread == 0)  //对方关闭套接字；
			break;
		nleft -= nread;
		ptr += nread;
	}
	    return(n-nleft);     //返回实际读到的字节数；
}

