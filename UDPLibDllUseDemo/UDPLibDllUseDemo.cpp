// UDPLibDllUseDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include "../UDPLib/SimpleUDPLib.h"
#pragma comment(lib, "Debug/udplibDLL.lib")
#include <windows.h>
int main()
{
	char buf[65507];
	int len = 100;
	while (1)
	{
		memset(buf, 0xAA, len);
		udplib_send("127.0.0.1:7777", "127.0.0.1:8888", buf, len);
		Sleep(100);
	}
    return 0;
}

