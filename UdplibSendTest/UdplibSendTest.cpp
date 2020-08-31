// UdplibSendTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../UDPLib/SimpleUDPLib.h"
#include <windows.h>

int main()
{
	char data[65536];
	int len = 507;
	while (1)
	{
		memset(data,len/500 , len);

		len += 500;

		udplib_send("127.0.0.1:7777", "127.0.0.1:8888", data, len);

		if (len >= 65507)
			len = 507;

		Sleep(100);
	}
	
	udplib_deinitialize();
    return 0;
}

