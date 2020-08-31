# simple_udp_lib
A simple UDP operation function library. You can use two or three functions to complete all UDP operations. Such as “udplib_send”, “udplib_recv” and "udplib_setblockmode".

# features
- Simple
- C and C + + compatible
- Cross platform (Windows vs and Linux gcc)
- Use IP and port numbers in string as nodes

# how to use
Add the source code or dynamic library to your project and include the header file "simpleudplib.h" to call the UDP operation functions.

# Function manual
## udplib_recv
UDP data receiving function.
```cpp
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
int udplib_recv(const char source[], const char target[], char recv_buf[], int max_datalen);
```
## udplib_send
UDP data sending function.
```cpp
//************************************
// Method:    udplib_send 
// FullName:  udplib send function
// Access:    public 
// Returns:   int 0 means send succeed，other undifined error num returned by sendto
// Qualifier:
// Parameter: char source[] source str, format as "xxx.xxx.xxx.xxx:xxxx"
// Parameter: char target[] dest str, format as "xxx.xxx.xxx.xxx:xxxx"
// Parameter: const char data[] send data buffer
// Parameter: int datalen the length of send buffer 0~65507
//************************************
int udplib_send(char source[], char target[], const char data[], int datalen);
```

## udplib_setblockmode
Set the blocking mode of the receive port.
Called before receiving function.
```cpp
//************************************
// Method:    udplib_setblockmode
// FullName:  the function to set node block mode 
// Access:    public 
// Returns:   int  0 means no error，other undefined num returned by ioctlsocket
// Qualifier:
// Parameter: const char addr[] format as "xxx.xxx.xxx.xxx:xxxx"
// Parameter: unsigned long mode 0-bolck，1-noblock
//************************************
int udplib_setblockmode(const char addr[],unsigned long mode);
```

## udplib_deinitialize
Library exit function.\
Call the function when the library is no longer in use.
```cpp
//************************************
// Method:    udplib_deinitialize
// FullName:  
// Access:    public 
// Returns:   int 0 means no error, other undefined num returned by WSACleanup
// Qualifier:
// Parameter: void
// Function: clean WSA，release all resource
//************************************
int udplib_deinitialize(void);
```

## udplib_initalize
Library initialization function.\
This function will be called by other operation functions and does not need to be called actively.
```cpp
//************************************
// Method:    udplib_initialize
// FullName:  udplib_initialize
// Access:    public 
// Returns:   int 0 means no error，other undefined num returned by WSAStartup
// Qualifier:
// Parameter: void
// Function: initalized net lib
//************************************
int udplib_initialize(void);
```