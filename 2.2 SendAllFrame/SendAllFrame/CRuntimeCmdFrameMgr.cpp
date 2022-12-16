#include "CRuntimeCmdFrameMgr.h"

CRuntimeCmdFrameMgr::CRuntimeCmdFrameMgr(std::shared_ptr<CUDPService> _udpservice) :
	ICmdFrameMgr(_udpservice)
{
	m_SendFrameSize = 15;
	m_RecvFrameSize = 47;

	m_SendFrame = new UInt8[m_SendFrameSize];
	m_RecvFrame = new UInt8[m_RecvFrameSize];
}

void CRuntimeCmdFrameMgr::ExecCommand(int _id)
{
	if (!m_CommandTimer.isActive()) {
		m_PDUStruct.N = m_SendFrameSize;
		m_PDUStruct.PF1 = 0x01;
		m_PDUStruct.PF2 = 0x06;
		m_PDUStruct.DA = 0x10 + _id - 1; //控制器代号1
		//第1字节数据：1
		m_PDUStruct.FrameData[0] = 1;
		m_CommandTimer.setInterval(m_CommandGap);
		m_CommandTimer.start();
	}
}

void CRuntimeCmdFrameMgr::_ParseFrame(UInt8* const _recvframe)
{
	for (int i = 0; i < 10;  ++i) {
		m_OpenTime[i] = _recvframe[i + 7];
	}
}

