#include <stdio.h>

#include "../UDPLib/SimpleUDPLib.h"
#include <windows.h>

#if 1 //使用QueryPerformanceCounter测量时间，精度最高为1/nFreq 秒，一般为us级别
#define INCLUDE_HEADER      <windows.h>
#define VAR_MACRO           LARGE_INTEGER t1,t2,nFreq;double elapsed_time = 0.0
#define TIME_METTER_INIT    QueryPerformanceFrequency(&nFreq)
#define GET_START_TIME      QueryPerformanceCounter(&t1)
#define GET_END_TIME        QueryPerformanceCounter(&t2)
#define GET_ELAPSED_TIME    elapsed_time = (t2.QuadPart - t1.QuadPart)*1000000.0/nFreq.QuadPart
#define PRINT_ELAPSED_TIME  printf("Time ELAPSED is %lf us\n", elapsed_time)
#endif
#include INCLUDE_HEADER
int main(int argc, char* argv[])
{
	VAR_MACRO;
	TIME_METTER_INIT;
	char buf[1024] = "udplib send data test\n";
	int buf_len = 1024;
	int count = 0;
	while (1)
	{
		GET_START_TIME;
		//udplib_send("127.0.0.1:1024", "127.0.0.1:8888", buf, strlen(buf));
		int recv_len = udplib_recv("127.0.0.1:8888","127.0.0.1:1024", buf, buf_len);
		GET_END_TIME;
		GET_ELAPSED_TIME;
		PRINT_ELAPSED_TIME;
		if (recv_len > 0)
		{
			buf[recv_len] = 0;
			printf("recvfrom:%s\n", buf);
		}
		count++;
		
		Sleep(1000);
	}
	
	return 0;

}
