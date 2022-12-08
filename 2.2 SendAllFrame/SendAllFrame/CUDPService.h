#pragma once

#include "datatype.h"

#include <WinSock2.h>
#include <iostream>

#pragma comment(lib,"ws2_32.lib")

/// @brief �ṩUDPͨ����ط���
class CUDPService
{
	//Q_OBJECT
public:
	/// @brief ���캯������ʼ��WSA��socket
	CUDPService();

	/// @brief ����Frame
	/// @param _frame ��Ҫ���͵�֡
	/// @return ���������ȷ������true
	bool SendFrame(const UInt8 _frame[256], const int _size, const int _controllerid);

	bool RecvFrame(UInt8* const _frame, const int _size, const int _controllerid);

	/// @brief ��������ip
	/// @param _hostip ����ip��Ĭ��Ϊ192.168.1.1
	/// @return ���������ȷ������true
	bool SetHostIP(const char _hostip[128] = { "192.168.1.1" });

	/// @brief ���ó�ʼ�����˿ڣ������˿ڼ��㷽ʽΪm_InitHostPort+m_ControllerID-1
	/// @param _port �����˿ڣ�Ĭ��Ϊ60100��ȡֵ��ΧΪ0-65535
	/// @return ���������ȷ������true
	bool SetInitHostPort(const int _port = 60100);

	/// @brief ���ó�ʼ������ip
	/// @param _hostip ������ip,Ĭ��Ϊ192.168.1.30
	/// @return ���������ȷ������true
	bool SetControllerIP(const char _ip[128] = { "192.168.1.30" });

	/// @brief ���ó�ʼ�������˿ڣ��������˿ڼ��㷽ʽΪm_InitControllerPort+m_ControllerID-1
	/// @param _port �����˿ڣ�Ĭ��Ϊ60000��ȡֵ��ΧΪ0-65535
	/// @return ���������ȷ������true
	bool SetInitControllerPort(const int _port = 60000);

	/// @brief ѡ�����������������ip��ַ�Ͷ�Ӧ�Ķ˿ںŰ󶨵�socket����һ����������
	/// @param _controllerid ������ID��ȡֵ��ΧΪ1-15 
	/// @return �����������������true
	bool ConnectSocket(const int _controllerid);

	/// @brief �Ͽ��Ϳ�����������
	/// @param _controllerid ������ID��ȡֵ��ΧΪ1-15
	/// @return �����������������true
	bool DisconncetSocket(const int _controllerid);

	/// @brief ���ĳid������������״̬
	/// @param _controllerid ������ID��ȡֵ��ΧΪ1-15
	/// @return ����ÿ����������ӵ�����������true�����򷵻�false
	bool ConnectStatus(const int _controllerid) const;

	/// @brief �����������ر�WSA��socket
	~CUDPService();



private:
	/// @brief ��֤ipv4��ַ�Ƿ�Ϸ���
	/// @param _ipv4 ����֤��ipv4��ַ
	/// @return �����������������true
	bool __CheckIPv4IsVaild(const char _ipv4[128]);


	// socket��ر���

	SOCKET m_HostSocket[cMaxConnection];
	sockaddr_in m_HostAddr;
	int m_HostAddrLen = sizeof(m_HostAddr);
	sockaddr_in m_ControllerAddr;
	int m_ControllerAddrLen = sizeof(m_ControllerAddr);

	// ip��port��ر���

	char m_HostIP[128] = {"192.168.1.1"}, m_ControllerIP[128] = {"192.168.1.30"};
	u_short m_InitHostPort { 60100 }, m_InitControllerPort {60000};

	// ���ӹ���
	bool m_bIsConnect[15] = { false };
};

