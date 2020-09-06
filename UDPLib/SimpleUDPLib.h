/***********************************************************************
* filename:SimpleUDPLib.h
*
* content:SimpleUDPLib Header
*
* author:yu_lei(2275655283@qq.com)
*
* create time:2020-08-07 23:52
* 
***********************************************************************/
#ifndef __SIMPLE_UDP_LIB_H__
#define __SIMPLE_UDP_LIB_H__
#ifdef __cplusplus
extern "C" {
#endif

#ifdef UDPLIBDLL_EXPORTS
	#define UDPLIBDLL_API __declspec(dllexport)
#else
	// if you want to use udplib directy by using source code,
	// define USE_UDP_LIB_SOURCE_CODE in your project first

	#ifdef USE_UDP_LIB_SOURCE_CODE
		#define UDPLIBDLL_API 
	#else
		#define UDPLIBDLL_API __declspec(dllimport)
	#endif
#endif

#if ((defined _WIN32) || (defined _WIN64))
//Check platform windows version
//include winsock files and libs
#include <winsock2.h>

// in order to avoid winsock2.h macro confilct
// you need include Windows.h after thsi header in your apps
#pragma comment(lib, "ws2_32.lib")

#else //code for other platform

//headers for linux socket program
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>

#define SOCKET int
#define ULONG unsigned long
#define USHORT unsigned short
#define SOCKADDR_IN struct sockaddr_in

#endif

/**********************error code defination***********************/
typedef enum udplib_errorcode{
	UDP_SOCKET_ERROR = -1,        // SOCKET_ERROR
	SOURCE_FORMAT_INVALID = -100, // invalid source string format
	TARGET_FORMAT_INVALID = -200, // invalid dest string format
	NODE_CREATE_FAILED = -300,    // failed to create node
	NODE_BIND_FAILED = -400,      // failed to bind node
	INITALIZED_FAILED = -500     // failed to inintalized
}UDBLIB_ERRORCODE;

/**************************function defination***************************/

//a simple udp operation lib, Specify the IP and port number then you can do udp operation

//************************************
// Method:    udplib_recv
// FullName:  udplib recv function
// Access:    public 
// Returns:   int -1 means recv failed, >0 represent the length of recved data
// Qualifier: yu_lei
// Parameter: const char source[] source str, format as "xxx.xxx.xxx.xxx:xxxx"
// Parameter: const char target[] dest str, format as "xxx.xxx.xxx.xxx:xxxx"
// Parameter: char recv_buf[]     recv buffer
// Parameter: int max_datalen     the length of recv buffer
//************************************
UDPLIBDLL_API int udplib_recv(const char source[], const char target[], char recv_buf[], int max_datalen);

//************************************
// Method:    udplib_send 
// FullName:  udplib send function
// Access:    public 
// Returns:   int 0 means send succeed, other undifined error num returned by sendto
// Qualifier:
// Parameter: char source[] source str, format as "xxx.xxx.xxx.xxx:xxxx"
// Parameter: char target[] dest str, format as "xxx.xxx.xxx.xxx:xxxx"
// Parameter: const char data[] send data buffer
// Parameter: int datalen the length of send buffer 0~65507
//************************************
UDPLIBDLL_API int udplib_send(char source[], char target[], const char data[], int datalen);

//aux function defination

//************************************
// Method:    udplib_initialize
// FullName:  udplib_initialize
// Access:    public 
// Returns:   int 0 means no error, other undefined num returned by WSAStartup
// Qualifier:
// Parameter: void
// Function: initalized net lib
//************************************
UDPLIBDLL_API int udplib_initialize(void);

//************************************
// Method:    udplib_deinitialize
// FullName:  
// Access:    public 
// Returns:   int 0 means no error, other undefined num returned by WSACleanup
// Qualifier:
// Parameter: void
// Function: cleanup WSA, release all resource
//************************************
UDPLIBDLL_API int udplib_deinitialize(void);

//************************************
// Method:    udplib_setblockmode
// FullName:  the function to set node block mode 
// Access:    public 
// Returns:   int  0 means no error, other undefined num returned by ioctlsocket
// Qualifier:
// Parameter: const char addr[] format as "xxx.xxx.xxx.xxx:xxxx"
// Parameter: unsigned long mode 0-bolck, 1-noblock
//************************************
UDPLIBDLL_API int udplib_setblockmode(const char addr[],unsigned long mode);


#ifdef __cplusplus
}
#endif
#endif
