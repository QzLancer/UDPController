#include "CUDPService.h"

CUDPService::CUDPService()
{
    //1、调用WSAStartup函数，设置程序中用到的Windows SOCKET版本，并初始化相应版本的库
    WSADATA wsadata;
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
        printf("Error: Failed. Error Code: %d\n", WSAGetLastError());
        exit(1);
    }

    //2、初始化host socket
    m_HostSocket = socket(AF_INET, SOCK_DGRAM, 0);  //ipv4，UDP方式连接
    if (m_HostSocket == INVALID_SOCKET) {
        printf("Error: Initial Socket falied\n");
        exit(1);
    }
}

bool CUDPService::SendFrame(const UInt8 _frame[256], const int _size)
{
    //控制器ip地址和端口号设置
    m_ControllerAddr.sin_family = AF_INET;
    m_ControllerAddr.sin_addr.S_un.S_addr = inet_addr(m_ControllerIP);
    m_ControllerAddr.sin_port = htons(m_InitControllerPort);
    printf("Controller setting finish. ip: %s, port: %d\n", inet_ntoa(m_ControllerAddr.sin_addr), ntohs(m_ControllerAddr.sin_port));

    //发送数据
    char sendbuf[256] = { 0 };
    memcpy(sendbuf, _frame, _size * sizeof(char));  //不能直接输出UInt8类型变量，因此需要将内容拷贝到char类型的数组中

    int sendlen = sendto(m_HostSocket, sendbuf, strlen(sendbuf), 0, (sockaddr*)&m_ControllerAddr, m_ControllerAddrLen);
    if (sendlen < 0) {
        printf("Message: Send frame error!\n");
        return false;
    }

    return true;
}

bool CUDPService::RecvFrame(UInt8* const _frame, const int _size)
{
    char recvbuf[256];
    int recvlen = recvfrom(m_HostSocket, recvbuf, _size * sizeof(char), 0, (sockaddr*)&m_ControllerAddr, &m_ControllerAddrLen);
    if (recvlen < 0) {
        printf("Message: Receive frame error!\n");
        return false;
    }

    memcpy(_frame, recvbuf, _size * sizeof(char));
    return true;
}

bool CUDPService::SetHostIP(const char _ip[128])
{
    if (__CheckIPv4IsVaild(_ip)) {
        memcpy(m_HostIP, _ip, 128 * sizeof(char));
        printf("Message: Set host IP successfully. Current host IP: %s\n", m_HostIP);
    }
    else {
        printf("Warning: Host IPv4 is invalid. Please reset host IPv4.\n");
    }
    return true;
}

bool CUDPService::SetInitHostPort(const int _port)
{
    if (_port > 0 && _port < 65535) {
        m_InitHostPort = _port;
        printf("Message: Set initial host port successfully. Current host port: %d\n", m_InitHostPort);
    }
    else {
        printf("Warning: Host port is invalid. Please reset host port.\n");
    }
    return true;
}

bool CUDPService::SetControllerIP(const char _ip[128])
{
    if (__CheckIPv4IsVaild(_ip)) {
        memcpy(m_ControllerIP, _ip, 128 * sizeof(char));
        printf("Message: Set controller IP successfully. Current controller IP: %s\n", m_ControllerIP);
    }
    else {
        printf("Warning: Controller IPv4 is invalid. Please reset controller IPv4.\n");
    }
    return true;
}

bool CUDPService::SetInitControllerPort(const int _port)
{
    if (_port >= 0 && _port <= 65535) {
        m_InitControllerPort = _port;
        printf("Message: Set initial controller port successfully. Current controller port: %d\n", m_InitControllerPort);
    }
    else {
        printf("Warning: Controller port is invalid. Please reset host port.\n");
    }
    return true;
}

bool CUDPService::SetControllerID(const int _controllerid)
{
    if (_controllerid >= 1 && _controllerid <= 15) {
        m_ControllerID = _controllerid;
        printf("Message: Set controller ID successfully. Current controller ID: %d\n", m_ControllerID);
    }
    else {
        printf("Warning: Controller ID is invalid. Please reset controller id.\n");
    }
    return true;
}

CUDPService::~CUDPService()
{
    closesocket(m_HostSocket);
    WSACleanup();
    printf("Message: Exist connection\n");
}

bool CUDPService::__CheckIPv4IsVaild(const char _ipv4[128])
{
    char s[128];
    memcpy(s, _ipv4, 128 * sizeof(char));
    int k = 0;        //记录每个segment起始位置
    int pCnt = 0;     //记录'.'的个数
    int size = 0;

    while (s[size] != '\0') {
        size++;
    }
    
    s[size] = '.';

    for (int i = 0; i < size+1; ++i) {
        if (s[i] == '.') {
            s[i] = '\0';    //方便atoi使用
            if (s[k] == '\0'                                //连续..或第一个为.的情况
                || (s[k] == '0' && strlen(&s[k]) > 1)         //以0开头的情况
                || !(atoi(&s[k]) <= 255 && atoi(&s[k]) >= 0)) //不符合区间范围
            {
                return false;
            }
            k = i + 1;
            ++pCnt;
        }
        else if (!(s[i] >= '0' && s[i] <= '9')) {            //包含非 0-9或'.' 的情况
            return false;
        }
    }

    if (pCnt != 3 + 1) return false;     //'.'不是3段,最后一个1是自己加的

    return true;
}

bool CUDPService::BindSocket( )
{
    m_HostAddr.sin_family = AF_INET;
    m_HostAddr.sin_addr.S_un.S_addr = inet_addr(m_HostIP);   //inet_addr()：将一个点分十进制的IP转换成一个长整型数，vs2015后需要取消SDL检查or预处理加上_WINSOCK_DEPRECATED_NO_WARNINGS才可用
    m_HostAddr.sin_port = htons(m_InitHostPort + m_ControllerID - 1); //atoi()：字符串转化成整形；htons()：主机字节顺序（低字节序）转变成网络字节顺序（高字节序）
    if (bind(m_HostSocket, (sockaddr*)&m_HostAddr, sizeof(sockaddr)) == SOCKET_ERROR) {
        perror("Bind error\n");
        return false;
    }
    printf("Host socket binding finish. ip: %s, port: %d\n", inet_ntoa(m_HostAddr.sin_addr), ntohs(m_HostAddr.sin_port));
    return true;
}
