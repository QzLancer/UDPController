#include "CUDPService.h"

CUDPService::CUDPService()
{
    ////1、调用WSAStartup函数，设置程序中用到的Windows SOCKET版本，并初始化相应版本的库
    //WSADATA wsadata;
    //if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
    //    printf("Error: Failed. Error Code: %d\n", WSAGetLastError());
    //    exit(1);
    //}

    ////2、初始化host socket
    //for (int i = 0; i < cMaxConnection; ++i) {
    //    m_HostSocket[i] = socket(AF_INET, SOCK_DGRAM, 0);  //ipv4，UDP方式连接
    //    if (m_HostSocket[i] == INVALID_SOCKET) {
    //        printf("Error: Initial Socket[%d] falied\n", i);
    //        exit(1);
    //    }
    //}
}

bool CUDPService::SendFrame(const UInt8 _frame[256], const int _size, const int _controllerid)
{
    if (m_bIsConnect == false) {
        printf("Message: Controller %d is not connected.", _controllerid);
        return false;
    }

    //发送数据
    char sendbuf[256] = { 0 };
    memcpy(sendbuf, _frame, _size * sizeof(char));  //不能直接输出UInt8类型变量，因此需要将内容拷贝到char类型的数组中

    //int sendlen = send(m_HostSocket[_controllerid - 1], sendbuf, _size * sizeof(char), 0);
    //if (sendlen < 0) {
    //    printf("Message: Send frame error!\n");
    //    return false;
    //}

    m_QSocket.writeDatagram(sendbuf, _size * sizeof(char), QHostAddress(m_ControllerIP), m_ControllerPort);

    return true;
}

//bool CUDPService::RecvFrame(UInt8* const _frame, const int _size, const int _controllerid)
//{
//    if (m_bIsConnect == false) {
//        printf("Message: Controller %d is not connected.", _controllerid);
//        return false;
//    }
//
//    char recvbuf[256] = {0};
//    
//    //int recvlen = recv(m_HostSocket[_controllerid - 1], recvbuf, _size * sizeof(char), 0);
//    //if (recvlen < 0) {
//    //    printf("Message: Receive frame error!\n");
//    //    return false;
//    //}
//
//
//
//    memcpy(_frame, recvbuf, _size * sizeof(char));
//    return true;
//}

bool CUDPService::SetHostIP(const char _ip[128])
{
    if (__CheckIPv4IsVaild(_ip)) {
        memcpy(m_HostIP, _ip, 128 * sizeof(char));
        printf("Message: Set host IP successfully. Current host IP: %s\n", m_HostIP);
    }
    else {
        printf("Warning: Host IPv4 is invalid. Please reset host IPv4.\n");
        return false;
    }
    return true;
}

bool CUDPService::SetInitHostPort(const int _port)
{
    if (_port > 0 && _port < 65535) {
        m_HostPort = _port;
        printf("Message: Set initial host port successfully. Current host port: %d\n", m_HostPort);
    }
    else {
        printf("Warning: Host port is invalid. Please reset host port.\n");
        return false;
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
        return false;
    }
    return true;
}

bool CUDPService::SetInitControllerPort(const int _port)
{
    if (_port >= 0 && _port <= 65535) {
        m_ControllerPort = _port;
        printf("Message: Set initial controller port successfully. Current controller port: %d\n", m_ControllerPort);
    }
    else {
        printf("Warning: Controller port is invalid. Please reset host port.\n");
        return false;
    }
    return true;
}

bool CUDPService::ConnectStatus() const
{
    return m_bIsConnect;
}

bool CUDPService::ConvertPDUtoFrame(PDUStruct _pdu, UInt8* const _frame)
{
    if (_pdu.N < 7) {
        printf("Error: Frame size is less than 7!");
        return false;
    }
    _frame[0] = _pdu.Head1;
    _frame[1] = _pdu.Head2;
    _frame[2] = _pdu.N;
    _frame[3] = _pdu.SA;
    _frame[4] = _pdu.DA;
    _frame[5] = _pdu.PF1;
    _frame[6] = _pdu.PF2;
    for (UInt8 i = 7; i < _pdu.N; ++i) {
        _frame[i] = _pdu.FrameData[i - 7];
    }

    return true;
}

bool CUDPService::SendPDU(PDUStruct _pdu, const int _controllerid)
{
    UInt8* frame = new UInt8[_pdu.N];
    if (!ConvertPDUtoFrame(_pdu, frame)) {
        printf("Error: ConvertPDUtoFrame failed.\n");
        return false;
    }
    if (!SendFrame(frame, _pdu.N, _controllerid)) {
        printf("Error: SendFrame failed.\n");
        return false;
    }
    delete[] frame;
    return true;
}

CUDPService::~CUDPService()
{
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

bool CUDPService::ConnectSocket()
{
    //如果已连接，直接返回
    if (m_bIsConnect == true)
        return true;

    // 建立socket连接
    //m_HostAddr.sin_family = AF_INET;
    //m_HostAddr.sin_addr.S_un.S_addr = inet_addr(m_HostIP);   //inet_addr()：将一个点分十进制的IP转换成一个长整型数，vs2015后需要取消SDL检查or预处理加上_WINSOCK_DEPRECATED_NO_WARNINGS才可用
    //m_HostAddr.sin_port = htons(m_HostPort + _controllerid - 1); //atoi()：字符串转化成整形；htons()：主机字节顺序（低字节序）转变成网络字节顺序（高字节序）
    //if (bind(m_HostSocket[_controllerid - 1], (sockaddr*)&m_HostAddr, sizeof(sockaddr)) == SOCKET_ERROR) {
    //    perror("Bind error\n");
    //    return false;
    //}

    if (!m_QSocket.bind(QHostAddress(m_HostIP), m_HostPort)) {
        perror("Error: Bind failed.\n");
        return false;
    }


    ////5、阻塞控制设置为100ms
    //timeval tv;
    //tv.tv_sec = 500;
    //tv.tv_usec = 0;
    //if (setsockopt(m_HostSocket[_controllerid - 1], SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv)) < 0) {
    //    printf("socket option SO_RCVTIMEO not support\n");
    //}

    //// 控制器ip地址和端口号设置
    //m_ControllerAddr.sin_family = AF_INET;
    //m_ControllerAddr.sin_addr.S_un.S_addr = inet_addr(m_ControllerIP);
    //m_ControllerAddr.sin_port = htons(m_ControllerPort + _controllerid - 1);
    //printf("Controller setting finish. ip: %s, port: %d\n", inet_ntoa(m_ControllerAddr.sin_addr), ntohs(m_ControllerAddr.sin_port));

    //// 连接到控制器
    //if (connect(m_HostSocket[_controllerid - 1], (sockaddr*)&m_ControllerAddr, m_ControllerAddrLen) == SOCKET_ERROR) {
    //    printf("Error: Connect to controller %d\n", _controllerid);
    //    DisconncetSocket(_controllerid);
    //    return false;
    //}

    m_bIsConnect = true;
    return true;
}

bool CUDPService::DisconncetSocket()
{
    if (m_bIsConnect == false)
        return true;

    //closesocket(m_HostSocket[_controllerid - 1]);

    m_QSocket.close();

    m_bIsConnect = false;
    return true;
}
