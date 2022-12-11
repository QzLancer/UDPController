#pragma once

#include "IFrameMgr.h"

#include <QObject>
#include <QtCore/QTimer>
#include <QtCore/QDateTime>

/// @brief ����������
class CHbFrameMgr : public IFrameMgr
{
	Q_OBJECT

public:
	explicit CHbFrameMgr(std::shared_ptr<CUDPService> _udpservice);

	/// @brief ��ʼ�������
	/// @param _id ������ID
	void StartHeartBeat(int _id);

	void StopHeartBeat(int _id);

protected:
	virtual void _ParseFrame(UInt8* const _recvframe) override;

private:
	QTimer m_HeartBeatTimer; //������ⶨʱ��
	const int m_HeartBeatGap = 1000; //1000ms����һ����������Timer

	/// @brief ��Ϣ����ʱ��
	UInt8 m_ReceivedTime[4] = {0};

	/// @brief �豸���µ�״̬��0-�ϵ� 1�ϵ磩Bit0-Bit5�ֱ�Ϊ1-6·������Bit6-Bit9Ϊ1-4·ֱ����Bit10-Bit15Ϊ����
	bool m_PowerState[10] = {false};

	/// @brief ���״̬��0x01-δ������0x02 - �Լ��У�0x04 - ������0x05 - ָ����Ӧ�У�0x07 - ���ϣ�0x08 - ������
	UInt8 CombineState = 0;



private slots:
	/// @brief �����������PDU
	void __slotSendHeartBeatPDU();
};

