#pragma once

#include "datatype.h"
#include "CUDPService.h"

#include <QObject>
#include <QtCore/QTimer>
#include <QtCore/QDateTime>

/// @brief ����ͬ����֡
class CHeartBeatManager : public QObject
{
	Q_OBJECT

public:
	explicit CHeartBeatManager(std::shared_ptr<CUDPService> _udpservice);

	/// @brief ��ʼ�������
	/// @param _id ������ID
	void StartHeartBeat(int _id);

	void StopHeartBeat(int _id);


private:
	std::shared_ptr<CUDPService> m_UDPService;

	PDUStruct m_HeartBeatPDU;

	QTimer m_HeartBeatTimer; //������ⶨʱ��
	const int m_HeartBeatGap = 1000; //1000ms����һ����������Timer
	int m_ControllerID = 1;	//��Ҫ������Ϣ�Ŀ�����id

private slots:
	/// @brief �����������PDU
	void __slotSendHeartBeatPDU();
};

