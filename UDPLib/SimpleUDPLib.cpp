/***********************************************************************
* filename:SimpleUDPLib.c
*
* content:SimpleUDPLib Header
*
* author:yu_lei(2275655283@qq.com)
*
* create time:2020-08-08 00:54
*
***********************************************************************/
#include <iostream>
#include <stdio.h>
#include <unordered_map>
#include <string>

using namespace std;

#include "SimpleUDPLib.h"

//***************** Definition of structure *********************//
typedef struct
{
	bool initalized;
	SOCKET node_socket;//node socket, 0 means uninitialized
	ULONG bind_ip;//Bound IP address, 0 means unbound
	USHORT bind_port;//Bound port number. 0 means unbound
	SOCKADDR_IN  node_sin;
	unsigned long blockmode;//Receive blocking mode, 0 means receiving blocking, non-0 value means non-block
}UDP_NODE_DATA_TYPE;

//***************** Variable definition *********************//
static bool udplib_initialized;//Determine whether the system has been initialized
unordered_map<string, UDP_NODE_DATA_TYPE> local_node_map;
unordered_map<string, UDP_NODE_DATA_TYPE> remote_node_map;

//***************** Definition of intrinsic functions *********************//
int udplib_initialize(void);//Initialize network library
int udplib_deinitialize(void);//Clean up network library

int udplib_ExtractNodeInfo(const char ipAddr_port[],int &ipaddr,unsigned short &out_port)
{	
	//Determine whether the address is legal and obtain the corresponding IP and port information
	int a = -1, b = -1, c = -1,d = -1;
	int port = 0;
	int parameter_num = sscanf(ipAddr_port, "%d.%d.%d.%d:%d", &a, &b, &c, &d, &port);
	if (parameter_num == 5)
	{	
		if ((a >= 0 && a <= 255)
			&& (b >= 0 && b <= 255)
			&& (c >= 0 && c <= 255)
			&& (d >= 0 && d <= 255))
		{
			//The number of parameters is correct and the IP address is valid
			//if (port >= 1024 && port <= 49151) {
				b = b + (a << 8);
				c = c + (b << 8);
				d = d + (c << 8);
				ipaddr = d;
				out_port = (unsigned short)port;
				return 0;
			//}

// 			printf("invalid port number:%d\n", port);
// 			printf("suggest port range is 1024 ~ 49151\n");
// 			return -1;////Port out of range
		}
		printf("invalid ip address:%d.%d.%d.%d\n", a,b,c,d);
		return -2;
	}
	printf("unmatched format as xxx.xxx.xxx.xxx:port\n");
	return -3;//The input string is not in the correct format
}


//************************************
// Method:    udplib_initialize
// FullName:  udplib_initialize
// Access:    public 
// Returns:   int 0 means no error£¬other undefined num returned by WSAStartup
// Qualifier:
// Parameter: void
// Function: initalized net lib
//************************************
UDPLIBDLL_API int udplib_initialize(void)
{
#ifdef _WIN32 || _WIN64
	if (!udplib_initialized) 
	{
		WSADATA wsaData;
		WORD sockVersion = MAKEWORD(2, 2);
		int ret = WSAStartup(sockVersion, &wsaData);
		if (ret != 0)
		{
			fprintf(stderr, "WSAStartup failed ! return code = %d\n", ret);
			return ret;
		}
		
		printf("========================================\n");
		printf("            udplib initalize            \n");
		printf("----------------------------------------\n");
		printf("Except Sockets version  : %d.%d\n", (int)HIBYTE(wsaData.wVersion), (int)LOBYTE(wsaData.wVersion));
		printf("Highest Sockets version : %d.%d\n", (int)HIBYTE(wsaData.wHighVersion), (int)LOBYTE(wsaData.wHighVersion));
		printf("szDescription           : %s\n", wsaData.szDescription);
		printf("szSystemStatus          : %s\n", wsaData.szSystemStatus);
		printf("----------------------------------------\n");
		
		udplib_initialized = true;
	}
	
	return 0;
#else
	sprintf(stderr,"Unimplemented function");
	fflush();
	return 0;
#endif
}

//************************************
// Method:    udplib_deinitialize
// FullName:  
// Access:    public 
// Returns:   int 0 means no error, other undefined num returned by WSACleanup
// Qualifier:
// Parameter: void
// Function: clean WSA£¬release all resource
//************************************
UDPLIBDLL_API int udplib_deinitialize(void)
{
#ifdef _WIN32 || _WIN64
	
	//Traverse connection Map and close all socket links in it
	for (auto it = local_node_map.begin(); it != local_node_map.end(); it++)
	{
		closesocket(it->second.node_socket);
	}
	
	//Release all node resources
	local_node_map.clear();
	remote_node_map.clear();
	
	if(udplib_initialized)
	{
		udplib_initialized = false;
		return WSACleanup();
	}
	
	return 0;

#else
	sprintf(stderr,"Unimplemented function");
	fflush();
	return 0;
#endif
}


int udplib_DestoryNode(char target[])
{
	//TODO: Close the corresponding connection
	int ipaddr;
	unsigned short port_num;
	if (udplib_ExtractNodeInfo(target, ipaddr, port_num) != 0)
	{

		return 0;
	}
	else
	{
		return 1;
	}
	
}

int udplib_CreateLocalNode(UDP_NODE_DATA_TYPE &node, int ipaddr, unsigned short port_num)
{
#ifdef _WIN32 || _WIN64
	node.node_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	node.bind_ip = ipaddr;
	node.bind_port = port_num;
	if (port_num != 0) {
		//If the port number is greater than 0, bind
		//If the input port number is equal to 0, the port will be requested automatically when sending
		sockaddr_in src_sin;
		src_sin.sin_family = AF_INET;
		src_sin.sin_port = htons(node.bind_port);
		src_sin.sin_addr.S_un.S_addr = htonl(node.bind_ip);//inet_addr("127.0.0.1");
		if (bind(node.node_socket,
			(sockaddr *)&src_sin, sizeof(src_sin)) == SOCKET_ERROR)
		{
			printf("port %d bind error !\n", port_num);
			closesocket(node.node_socket);
			return NODE_BIND_FAILED;//Binding error
		}
		node.node_sin = src_sin;
	}
	node.initalized = true;
	return 0;

#else
	sprintf(stderr,"Unimplemented function");
	fflush();
	return 0;
#endif

}
int udplib_CreateRemoteNode(UDP_NODE_DATA_TYPE &node, int ipaddr, unsigned short port_num)
{
#ifdef _WIN32 || _WIN64
	node.node_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	node.bind_ip = ipaddr;
	node.bind_port = port_num;
	
	sockaddr_in node_sin;
	node_sin.sin_family = AF_INET;
	node_sin.sin_port = htons(node.bind_port);
	node_sin.sin_addr.S_un.S_addr = htonl(node.bind_ip);//inet_addr("127.0.0.1");
	node.node_sin = node_sin;
	
	node.initalized = true;

	return 0;
#else
	sprintf(stderr,"Unimplemented function");
	fflush();
	return 0;
#endif
}

//************************************
// Method:    udplib_send 
// FullName:  udplib send function
// Access:    public 
// Returns:   int 0 means send succeed£¬other undifined error num returned by sendto
// Qualifier:
// Parameter: char source[] source str, format as "xxx.xxx.xxx.xxx:xxxx"
// Parameter: char target[] dest str, format as "xxx.xxx.xxx.xxx:xxxx"
// Parameter: const char data[] send data buffer
// Parameter: int datalen the length of send buffer 0~65507
//************************************
UDPLIBDLL_API int udplib_send(char source[], char target[], const char data[], int datalen)
{
#ifdef _WIN32 || _WIN64
	string src_str = source;
	string dst_str = target;
	//Determine whether wsdata has been initialized. If not, initialize it
	if (!udplib_initialized) {
		int ret = udplib_initialize();
		if (ret != 0) {
			//Failed to create node
			printf("udplib_initialize failed with wsa error code = %d\n", ret);
			return ret;
		}
	}

	//Determine whether the send port has been created. If not, create it
	int ipaddr;
	unsigned short port_num;
	UDP_NODE_DATA_TYPE src_node, dst_node;
	//Query whether the target has the corresponding socket information, if not, create the corresponding socket
	src_node = local_node_map[src_str];
	if (!src_node.initalized)
	{
		if (udplib_ExtractNodeInfo(source, ipaddr, port_num) != 0)
			return SOURCE_FORMAT_INVALID;//Incorrect source address
		//The send port is not initialized. Initialize the port
		int ret = udplib_CreateLocalNode(src_node, ipaddr, port_num);
		if (ret == 0)//Node created successfully
			local_node_map[src_str] = src_node;
		else
		{
			printf("node [%s] create failed!\n", source);
			return NODE_CREATE_FAILED;
		}
	}
	//Get destination IP and port number
	dst_node = remote_node_map[dst_str];
	if (!dst_node.initalized)
	{
		if (udplib_ExtractNodeInfo(target, ipaddr, port_num) != 0)
			return TARGET_FORMAT_INVALID;//The destination address is incorrect
		int ret = udplib_CreateRemoteNode(dst_node, ipaddr, port_num);
		if (ret == 0)
		{
			remote_node_map[dst_str] = dst_node;
		}
		else {
			return NODE_CREATE_FAILED;
		}
		
	}
	int dst_len = sizeof(dst_node.node_sin);
	//send data
	int ret = sendto(src_node.node_socket, data, datalen, 0, (sockaddr *)&dst_node.node_sin, dst_len);
	return ret;
#else
	sprintf(stderr,"Unimplemented function");
	fflush();
	return 0;
#endif
}


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
UDPLIBDLL_API int udplib_recv(const char source[], const char target[], char recv_buf[], int max_datalen)
{
#ifdef _WIN32 || _WIN64
	string src_str = source;
	string dst_str = target;
	//Determine whether wsdata has been initialized. If not, initialize it
	if (!udplib_initialized) {
		int ret = udplib_initialize();
		if (ret != 0) {
			//Failed to create node
			printf("udplib_initialize failed with wsa error code = %d\n", ret);
			return INITALIZED_FAILED;
		}
	}

	//Determine whether the send port has been created. If not, create it
	int ipaddr;
	unsigned short port_num;
	UDP_NODE_DATA_TYPE src_node, dst_node;
	//Determine whether the source string format is correct
	src_node = remote_node_map[src_str];
	if (!src_node.initalized)
	{
		if (udplib_ExtractNodeInfo(source, ipaddr, port_num) != 0)
			return SOURCE_FORMAT_INVALID;//Incorrect source address
		//The send port is not initialized. Initialize the port
		int ret = udplib_CreateRemoteNode(src_node, ipaddr, port_num);
		if (ret == 0)//Node created successfully
			remote_node_map[src_str] = src_node;
		else
			return NODE_CREATE_FAILED;
	}
	
	//Determine whether the target node has been created. If not, create it
	dst_node = local_node_map[dst_str];
	if (!dst_node.initalized)
	{
		if (udplib_ExtractNodeInfo(target, ipaddr, port_num) != 0)
			return TARGET_FORMAT_INVALID;//The destination address is incorrect
		int ret = udplib_CreateLocalNode(dst_node, ipaddr, port_num);
		if (ret == 0)
		{
			local_node_map[dst_str] = dst_node;
		}
		else {
			return NODE_CREATE_FAILED;
		}
	}
	int sin_len = sizeof(src_node.node_sin);
	//receive data
	int ret = recvfrom(dst_node.node_socket, recv_buf, max_datalen, 0, (sockaddr *)&(src_node.node_sin), &sin_len);
	
	return ret;
#else
	sprintf(stderr,"Unimplemented function");
	fflush();
	return 0;
#endif
}

//************************************
// Method:    udplib_setblockmode
// FullName:  the function to set node block mode 
// Access:    public 
// Returns:   int  0 means no error£¬other undefined num returned by ioctlsocket
// Qualifier:
// Parameter: const char addr[] format as "xxx.xxx.xxx.xxx:xxxx"
// Parameter: unsigned long mode 0-bolck£¬1-noblock
//************************************
UDPLIBDLL_API int udplib_setblockmode(const char addr[], unsigned long mode)
{
#ifdef _WIN32 || _WIN64
	if (!udplib_initialized) {
		int ret = udplib_initialize();
		if (ret != 0) {
			//Failed to create node
			printf("udplib_initialize failed with wsa error code = %d\n", ret);
			return INITALIZED_FAILED;
		}
	}

	string dst_str = addr;
	int ipaddr;
	unsigned short port_num;
	UDP_NODE_DATA_TYPE dst_node;
	//Query whether the target has the corresponding socket information, if not, create the corresponding socket
	dst_node = local_node_map[dst_str];
	if (!dst_node.initalized)
	{
		if (udplib_ExtractNodeInfo(addr, ipaddr, port_num) != 0)
			return SOURCE_FORMAT_INVALID;//Incorrect source address
					 //The send port is not initialized. Initialize the port
		int ret = udplib_CreateLocalNode(dst_node, ipaddr, port_num);
		if (ret == 0)//Node created successfully
			local_node_map[dst_str] = dst_node;
		else
		{
			printf("node [%s] create failed!\n", addr);
			return NODE_CREATE_FAILED;
		}
	}
	dst_node.blockmode = mode;
	int ret = ioctlsocket(dst_node.node_socket, FIONBIO, &(dst_node.blockmode));
	if (ret != NO_ERROR) {
		printf("ioctlsocket failed with error: %ld\n", ret);
		return ret;
	}
	dst_node.blockmode = mode;
	local_node_map[dst_str] = dst_node;
	return 0;
#else
	sprintf(stderr,"Unimplemented function");
	fflush();
	return 0;
#endif
}


