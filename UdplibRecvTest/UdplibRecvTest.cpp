// UdplibRecvTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
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
#define PRINT_ELAPSED_TIME_MS  printf("Time ELAPSED is %lf ms\n", elapsed_time/1000.0)
#endif
int main()
{
	char data[65507] = { 0 };
	VAR_MACRO;
	TIME_METTER_INIT;
	GET_START_TIME;
	int count = 0;
	while (1)
	{
		count++;
		int ret = udplib_recv("127.0.0.1:8888", "127.0.0.1:9999", data, 65507);
		
		if (ret > 0) {
			data[ret] = 0;
			printf("udplib_recv:%s\n", data);
			GET_END_TIME;
			GET_ELAPSED_TIME;
			PRINT_ELAPSED_TIME_MS;
			GET_START_TIME;
		}
		
		if (count > 50) {
			udplib_setblockmode("127.0.0.1:9999", 1);
		}
		if (count > 50000) {
			break;
		}
	}
    return 0;
}

