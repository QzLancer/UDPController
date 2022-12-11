#include "CResetCmdFrameMgr.h"

CResetCmdFrameMgr::CResetCmdFrameMgr(std::shared_ptr<CUDPService> _udpservice) :
	ICmdFrameMgr(_udpservice)
{
	m_SendFrameSize = 15;
	m_RecvFrameSize = 15;

	m_SendFrame = new UInt8[m_SendFrameSize];
	m_RecvFrame = new UInt8[m_RecvFrameSize];
}

void CResetCmdFrameMgr::ExecCommand(int _id, bool _deviceselect[10], bool _isonpower)
{
	if (!m_CommandTimer.isActive()) {
		m_PDUStruct.N = m_SendFrameSize;
		m_PDUStruct.PF1 = 0x01;
		m_PDUStruct.PF2 = 0xFF;
		m_PDUStruct.DA = 0x10 + _id - 1; //控制器代号1
		//第1-2字节数据：0x55
		m_PDUStruct.FrameData[0] = 0x55;
		m_PDUStruct.FrameData[1] = 0x55;
		m_CommandTimer.start(m_CommandGap);
	}
}

void CResetCmdFrameMgr::_ParseFrame(UInt8* const _recvframe)
{
	if (_recvframe[9] = 0x55) {
		m_bIsSuccess = true;
	}
	else {
		m_bIsSuccess = false;
	}
}
