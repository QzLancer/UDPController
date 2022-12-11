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

protected:
	/// @brief �Բ�ͬ���͵�֡���н���
	/// @param _recvframe ���ܵ���֡
	virtual void _ParseFrame(UInt8* const _recvframe) = 0;

	std::shared_ptr<CUDPService> m_UDPService;

	PDUStruct m_PDUStruct;

	int m_ControllerID = 1;	//��Ҫ������Ϣ�Ŀ�����id
};

