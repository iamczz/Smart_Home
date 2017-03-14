/****************************************
 >        向套接字中指定写入n个字节
 >		  成功：返回实际写入的字节数
 >        失败：返回-1
 ****************************************/
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

ssize_t writen(int fd, const void *vptr, size_t n)
{
	size_t nleft;
	ssize_t nwritten;
	const char *ptr = vptr;
	
	nleft = n;
	while(nleft > 0)
	{
		if((nwritten = write(fd, ptr, nleft)) <= 0)
		{
			if(nwritten < 0 && errno == EINTR)
				nwritten = 0;
			else
				return -1;
		}
		nleft -= nwritten;
		ptr += nwritten;
	}
	
	return (n - nleft);
}
