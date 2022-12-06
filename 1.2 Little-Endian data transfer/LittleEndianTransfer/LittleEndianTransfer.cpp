// LittleEndianTransfer.cpp : 通过设备自检指令测试小端传输，主机端
// 2022/12/5
// By QzLancer


#include <iostream>
#include <stdint.h>
#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib") 

// 特定数据类型的定义
#define Int8 int8_t
#define UInt8 uint8_t  
#define Int16 int16_t
#define UInt16 uint16_t
#define Int32 int32_t
#define UInt32 uint32_t
#define Int64 int64_t


/// @brief 定义协议数据单元（PDU）结构体
struct PDUStruct {
    UInt8 Head1 = 0xA5;    //帧头1
    UInt8 Head2 = 0x5A;    //帧头2
    UInt8 N = 0;    //帧长度
    UInt8 SA = 0;    //源ID
    UInt8 DA = 0;    //目标ID
    UInt8 PF1 = 0;  //功能码1
    UInt8 PF2 = 0;  //功能码2
    UInt8 FrameData[255] = {'\0'}; //帧数据
};

SOCKET HostSocket;
sockaddr_in HostAddr;
int HostAddrLen = sizeof(HostAddr);
sockaddr_in ControllerAddr;
int ControllerAddrLen = sizeof(ControllerAddr);
UInt8 N = 1;   //控制器代号

/// @brief 初始化UDP连接
/// @param _hostip 主机IP
/// @param _hostport 主机端口
/// @param _controllerip 控制器IP
/// @param _controllerport 控制器端口
/// @return 如果程序正确，返回0
int InitializeUDP(const char* _hostip, u_short _hostport, const char* _controllerip, u_short _controllerport) {
    //1、调用WSAStartup函数，设置程序中用到的Windows SOCKET版本，并初始化相应版本的库
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Error: Failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    //2、初始化host socket
    HostSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (HostSocket == INVALID_SOCKET) {
        printf("Socket error\n");
        return 1;
    }

    //3、绑定主机ip地址和端口号
    HostAddr.sin_family = AF_INET;
    HostAddr.sin_addr.S_un.S_addr = inet_addr(_hostip);   //inet_addr()：将一个点分十进制的IP转换成一个长整型数，vs2015后需要取消SDL检查or预处理加上_WINSOCK_DEPRECATED_NO_WARNINGS才可用
    HostAddr.sin_port = htons(_hostport); //atoi()：字符串转化成整形；htons()：主机字节顺序（低字节序）转变成网络字节顺序（高字节序）
    if (bind(HostSocket, (sockaddr*)&HostAddr, sizeof(sockaddr)) == SOCKET_ERROR) {
        perror("Bind error\n");
        return 1;
    }

    printf("Host socket binding finish. ip: %s, port: %d\n", inet_ntoa(HostAddr.sin_addr), ntohs(HostAddr.sin_port));

    //4、控制器ip地址和端口号设置
    ControllerAddr.sin_family = AF_INET;
    ControllerAddr.sin_addr.S_un.S_addr = inet_addr(_controllerip);
    ControllerAddr.sin_port = htons(_controllerport);
    printf("Controller setting finish. ip: %s, port: %d\n", inet_ntoa(ControllerAddr.sin_addr), ntohs(ControllerAddr.sin_port));

    return 0;
}


/// @brief 先将PDU结构体转化为需要发送的帧，再发送数据帧给控制器（如何确保是小端传输呢？或许应该写一个大端和小端的转换函数）
/// @param _pdustruct 需要发送的PDU结构体
/// @return 如果程序正确，返回0
int SendFrame(PDUStruct _pdustruct) {
    if (_pdustruct.N < 7) {
        printf("Error: Frame size is less than 7!");
        return 1;
    }

    //将PDU结构体转化为需要发送的帧
    UInt8* Frame = new UInt8[_pdustruct.N];
    Frame[0] = _pdustruct.Head1;
    Frame[1] = _pdustruct.Head2;
    Frame[2] = _pdustruct.N;
    Frame[3] = _pdustruct.SA;
    Frame[4] = _pdustruct.DA;
    Frame[5] = _pdustruct.PF1;
    Frame[6] = _pdustruct.PF2;
    for (UInt8 i = 7; i < _pdustruct.N; ++i) {
        Frame[i] = _pdustruct.FrameData[i - 7];
    }

    //发送数据帧给控制器
    char sendBuf[256] = { 0 };
    memcpy(sendBuf, Frame, _pdustruct.N * sizeof(char));  //不能直接输出UInt8类型变量，因此需要将内容拷贝到char类型的数组中
    int sendLen = sendto(HostSocket, sendBuf, _pdustruct.N * sizeof(char), 0, (sockaddr*)&ControllerAddr, ControllerAddrLen);
    if (sendLen > 0) {
        printf("Send command to controller successfully!\n");
    }

    if (Frame != NULL) {
        delete[] Frame;
    }
    return 0;
}

int main()
{
    PDUStruct SelfCheckPDU;  //设备自检PDU
    SelfCheckPDU.N = 15;
    SelfCheckPDU.SA = 0x01;
    SelfCheckPDU.DA = 0x10 + N - 1; //控制器代号1
    SelfCheckPDU.PF1 = 0x01;
    SelfCheckPDU.PF2 = 0x02;
    for (int i = 0; i < 8; ++i) {
        SelfCheckPDU.FrameData[i] = 0x00;
    }
    if (InitializeUDP("192.168.199.235", 60100+N-1, "192.168.199.235", 60000+N-1)) {
        printf("Error: Initialize UDP falied!");
        return -1;
    }
    
    //发送PDU数据帧给设备
    while (1) {
        char command[256];
        printf("Enter Command: ");
        fgets(command, 256, stdin);
        if (strcmp(command, "exit\n") == 0) {
            break;
        }
        else if (strcmp(command, "selfcheck\n") == 0) {
            SendFrame(SelfCheckPDU);
        }
    }

    closesocket(HostSocket);
    WSACleanup();
    printf("Message: Exist connection");
    return 0;
}

