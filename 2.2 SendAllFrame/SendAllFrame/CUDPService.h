#pragma once

#include "datatype.h"

#include <WinSock2.h>
#include <iostream>

#pragma comment(lib,"ws2_32.lib")

/// @brief 提供UDP通信相关服务
class CUDPService
{
	//Q_OBJECT
public:
	/// @brief 构造函数，初始化WSA和socket
	CUDPService();

	/// @brief 发送Frame
	/// @param _frame 需要发送的帧
	/// @return 如果程序正确，返回true
	bool SendFrame(const UInt8 _frame[256], const int _size, const int _controllerid);

	bool RecvFrame(UInt8* const _frame, const int _size, const int _controllerid);

	/// @brief 设置主机ip
	/// @param _hostip 主机ip，默认为192.168.1.1
	/// @return 如果程序正确，返回true
	bool SetHostIP(const char _hostip[128] = { "192.168.1.1" });

	/// @brief 设置初始主机端口，主机端口计算方式为m_InitHostPort+m_ControllerID-1
	/// @param _port 主机端口，默认为60100，取值范围为0-65535
	/// @return 如果程序正确，返回true
	bool SetInitHostPort(const int _port = 60100);

	/// @brief 设置初始控制器ip
	/// @param _hostip 控制器ip,默认为192.168.1.30
	/// @return 如果程序正确，返回true
	bool SetControllerIP(const char _ip[128] = { "192.168.1.30" });

	/// @brief 设置初始控制器端口，控制器端口计算方式为m_InitControllerPort+m_ControllerID-1
	/// @param _port 主机端口，默认为60000，取值范围为0-65535
	/// @return 如果程序正确，返回true
	bool SetInitControllerPort(const int _port = 60000);

	/// @brief 选择控制器，并将主机ip地址和对应的端口号绑定到socket（绑定一个控制器）
	/// @param _controllerid 控制器ID，取值范围为1-15 
	/// @return 如果程序正常，返回true
	bool ConnectSocket(const int _controllerid);

	/// @brief 断开和控制器的连接
	/// @param _controllerid 控制器ID，取值范围为1-15
	/// @return 如果程序正常，返回true
	bool DisconncetSocket(const int _controllerid);

	/// @brief 获得某id控制器的连接状态
	/// @param _controllerid 控制器ID，取值范围为1-15
	/// @return 如果该控制器以连接到主机，返回true，否则返回false
	bool ConnectStatus(const int _controllerid) const;

	/// @brief 析构函数，关闭WSA和socket
	~CUDPService();



private:
	/// @brief 验证ipv4地址是否合法，
	/// @param _ipv4 待验证的ipv4地址
	/// @return 如果程序正常，返回true
	bool __CheckIPv4IsVaild(const char _ipv4[128]);


	// socket相关变量

	SOCKET m_HostSocket[cMaxConnection];
	sockaddr_in m_HostAddr;
	int m_HostAddrLen = sizeof(m_HostAddr);
	sockaddr_in m_ControllerAddr;
	int m_ControllerAddrLen = sizeof(m_ControllerAddr);

	// ip和port相关变量

	char m_HostIP[128] = {"192.168.1.1"}, m_ControllerIP[128] = {"192.168.1.30"};
	u_short m_InitHostPort { 60100 }, m_InitControllerPort {60000};

	// 连接管理
	bool m_bIsConnect[15] = { false };
};

