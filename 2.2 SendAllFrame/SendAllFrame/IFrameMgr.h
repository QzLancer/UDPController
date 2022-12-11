#pragma once

#include "datatype.h"
#include "CUDPService.h"
#include <QObject>

/// @brief ֡���������
class IFrameMgr : public QObject
{
	Q_OBJECT

public:
	explicit IFrameMgr(std::shared_ptr<CUDPService> _udpservice);

	virtual ~IFrameMgr();

protected:
	/// @brief �Բ�ͬ���͵�֡���н���
	/// @param _recvframe ���ܵ���֡
	virtual void _ParseFrame(UInt8* const _recvframe) = 0;

	std::shared_ptr<CUDPService> m_UDPService;

	PDUStruct m_PDUStruct;

	int m_ControllerID = 1;	//��Ҫ������Ϣ�Ŀ�����id


	int m_SendFrameSize = 0;
	int m_RecvFrameSize = 0;

	UInt8* m_SendFrame = NULL;
	UInt8* m_RecvFrame = NULL;
};

