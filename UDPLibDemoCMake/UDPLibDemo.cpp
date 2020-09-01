//============================================================================
// Name        : UDPLibDemo.cpp
// Author      : yu_lei
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "SimpleUDPLib.h"

using namespace std;

int main() {
	char recv_buf[1024];
	
	udplib_initialize();
	udplib_send("192.168.1.107:8888","192.168.1.100:9999","send data\n",10);
	udplib_setblockmode("192.168.1.107:8888",0);
	// udplib_recv();
	while(1)
	{
		int recv_len;
		udplib_send("192.168.1.107:8888", "192.168.1.100:9999", "send data\n",10);
		recv_len = udplib_recv("192.168.1.100:9999", "192.168.1.107:8888",recv_buf,1024);
		if(recv_len > 0)
		{
			recv_buf[recv_len] = '\0';
			printf("recv data : %s\n", recv_buf);
		}
		sleep(1);
	}
	return 0;
}

