#pragma once

#include "IFrameMgr.h"

#include <QObject>
#include <QtCore/QTimer>
#include <QtCore/QDateTime>

/// @brief 心跳监测管理
class CHbFrameMgr : public IFrameMgr
{
	Q_OBJECT

public:
	explicit CHbFrameMgr(std::shared_ptr<CUDPService> _udpservice);

	/// @brief 开始心跳监测
	/// @param _id 控制器ID
	void StartHeartBeat(int _id);

	void StopHeartBeat(int _id);

protected:
	virtual void _ParseFrame(UInt8* const _recvframe) override;

private:
	QTimer m_HeartBeatTimer; //心跳监测定时器
	const int m_HeartBeatGap = 1000; //1000ms发送一次心跳监测的Timer

	/// @brief 信息产生时间
	UInt8 m_ReceivedTime[4] = {0};

	/// @brief 设备上下电状态（0-断电 1上电）Bit0-Bit5分别为1-6路交流，Bit6-Bit9为1-4路直流，Bit10-Bit15为备用
	bool m_PowerState[10] = {false};

	/// @brief 组合状态：0x01-未开启；0x02 - 自检中；0x04 - 就绪；0x05 - 指令响应中；0x07 - 故障；0x08 - 开启中
	UInt8 CombineState = 0;



private slots:
	/// @brief 发送心跳监测PDU
	void __slotSendHeartBeatPDU();
};

