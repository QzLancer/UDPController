#include "CCommCmdFrameMgr.h"

CCommCmdFrameMgr::CCommCmdFrameMgr(std::shared_ptr<CUDPService> _udpservice):
	ICmdFrameMgr(_udpservice)
{
	m_SendFrameSize = 15;
	m_RecvFrameSize = 15;

	m_SendFrame = new UInt8[m_SendFrameSize];
	m_RecvFrame = new UInt8[m_RecvFrameSize];
}

void CCommCmdFrameMgr::ExecCommand(int _id, bool _isforbidsend)
{
	if (!m_CommandTimer.isActive()) {
		m_PDUStruct.N = m_SendFrameSize;
		m_PDUStruct.PF1 = 0x01;
		m_PDUStruct.PF2 = 0x03;
		m_PDUStruct.DA = 0x10 + _id - 1; //控制器代号1
		m_PDUStruct.FrameData[0] = _isforbidsend;
		m_CommandTimer.setInterval(m_CommandGap);
		m_CommandTimer.start();
	}
}

void CCommCmdFrameMgr::_ParseFrame(UInt8* const _recvframe)
{
	// 第8字节数据：通信状态
	m_CommStatus = _recvframe[7];

	// 第9字节数据：CAN_Error
	for (int i = 0; i < 8; ++i) {
		m_CAN_Error[i] = _recvframe[8] % 2;
		_recvframe[8] >>= 1;
	}
}
