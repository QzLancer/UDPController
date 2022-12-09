#pragma once

#include "datatype.h"
#include "CUDPService.h"

#include <QObject>
#include <QtCore/QTimer>
#include <QtCore/QDateTime>

/// @brief 处理同步类帧
class CHeartBeatManager : public QObject
{
	Q_OBJECT

public:
	explicit CHeartBeatManager(std::shared_ptr<CUDPService> _udpservice);

	/// @brief 开始心跳监测
	/// @param _id 控制器ID
	void StartHeartBeat(int _id);

	void StopHeartBeat(int _id);


private:
	std::shared_ptr<CUDPService> m_UDPService;

	PDUStruct m_HeartBeatPDU;

	QTimer m_HeartBeatTimer; //心跳监测定时器
	const int m_HeartBeatGap = 1000; //1000ms发送一次心跳监测的Timer
	int m_ControllerID = 1;	//需要发送消息的控制器id

private slots:
	/// @brief 发送心跳监测PDU
	void __slotSendHeartBeatPDU();
};

