#pragma once

#include "datatype.h"
#include "CUDPService.h"
#include <QObject>

/// @brief 帧管理抽象类
class IFrameMgr : public QObject
{
	Q_OBJECT

public:
	explicit IFrameMgr(std::shared_ptr<CUDPService> _udpservice);

protected:
	/// @brief 对不同类型的帧进行解析
	/// @param _recvframe 接受到的帧
	virtual void _ParseFrame(UInt8* const _recvframe) = 0;

	std::shared_ptr<CUDPService> m_UDPService;

	PDUStruct m_PDUStruct;

	int m_ControllerID = 1;	//需要发送消息的控制器id
};

