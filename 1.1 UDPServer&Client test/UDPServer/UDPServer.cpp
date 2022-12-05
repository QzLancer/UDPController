// UDPServer.cpp : UDP虚拟控制器，通过轮询方式读取虚拟主机指令，返回数据给虚拟主机。
// 2022/12/5
// By QzLancer
//


#include <iostream>
#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib") 


/// @brief 
/// @param argc 3个参数
/// @param argv 参数1：可执行文件路径（不需要在控制台输入）；参数2：控制器ip地址；参数3：控制器端口号
/// @return 
int main(int argc, char* argv[])
{
    //1、判断输入是否为2个参数（ip+port）
    if (argc != 3)
    {
        printf("Usage: %s <port>\n", argv[0]);
        return -1;
    }

    //2、调用WSAStartup函数，设置程序中用到的Windows SOCKET版本，并初始化相应版本的库
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed. Error Code: %d\n", WSAGetLastError());
        return -1;
    }

    //3、初始化controller socket
    SOCKET ControllerSocket;
    ControllerSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (ControllerSocket == INVALID_SOCKET) {
        perror("Socket error\n");
        return -1;
    }

    //4、绑定占用ip地址和端口号
    sockaddr_in ControllerAddr;
    ControllerAddr.sin_family = AF_INET;
    ControllerAddr.sin_addr.S_un.S_addr = inet_addr(argv[1]);   //inet_addr()：将一个点分十进制的IP转换成一个长整型数，vs2015后需要取消SDL检查or预处理加上_WINSOCK_DEPRECATED_NO_WARNINGS才可用
    ControllerAddr.sin_port = htons(atoi(argv[2])); //atoi()：字符串转化成整形；htons()：主机字节顺序（低字节序）转变成网络字节顺序（高字节序）
    if (bind(ControllerSocket, (sockaddr*)&ControllerAddr, sizeof(sockaddr)) == SOCKET_ERROR) {
        perror("Bind error\n");
        return -1;
    }
    printf("Control socket binding finish. ip: %s, port: %d\n", inet_ntoa(ControllerAddr.sin_addr), ntohs(ControllerAddr.sin_port));

    //5、接收和发送
    
    char sendBuf[1024] = "Nice to meet you too!\n";
    sockaddr_in HostAddr;
    int HostAddrLen = sizeof(HostAddr);
    while (1)
    {
        char recvBuf[1024] = { 0 };
        int sendLen = 0;
        int recvLen = recvfrom(ControllerSocket, recvBuf, 1024, 0, (sockaddr*)&HostAddr, &HostAddrLen);
        if (recvLen > 0) {
            printf("Receive a connection. ip: %s, port: %d\n", inet_ntoa(HostAddr.sin_addr), ntohs(HostAddr.sin_port));
            printf("Receive a message: %s", recvBuf);
        }

        if (strcmp(recvBuf, "Nice to meet you!\n") == 0) {
            sendLen = sendto(ControllerSocket, sendBuf, strlen(sendBuf), 0, (sockaddr*)&HostAddr, HostAddrLen);
        }
        if (sendLen > 0) {
            printf("Send message to host: %s\n", sendBuf);
        }
    }

    //6、关闭socket
    closesocket(ControllerSocket);
    WSACleanup();

    return 0;
}

