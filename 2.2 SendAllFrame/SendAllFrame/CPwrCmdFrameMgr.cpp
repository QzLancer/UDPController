#include "CPwrCmdFrameMgr.h"

CPwrCmdFrameMgr::CPwrCmdFrameMgr(std::shared_ptr<CUDPService> _udpservice) :
	ICmdFrameMgr(_udpservice)
{
	m_SendFrameSize = 15;
	m_RecvFrameSize = 15;

	m_SendFrame = new UInt8[m_SendFrameSize];
	m_RecvFrame = new UInt8[m_RecvFrameSize];
}

void CPwrCmdFrameMgr::ExecCommand(int _id, bool _deviceselect[10], bool _isonpower)
{
	if (!m_CommandTimer.isActive()) {
		m_PDUStruct.N = m_SendFrameSize;
		m_PDUStruct.PF1 = 0x01;
		m_PDUStruct.PF2 = 0x05;
		m_PDUStruct.DA = 0x10 + _id - 1; //控制器代号1
		//第1-2字节数据：设备选择（这个拷贝会出错吗？没有测试）
		memcpy(&m_PDUStruct.FrameData, _deviceselect, 10 * sizeof(bool));
		//第3字节数据：上电控制
		m_PDUStruct.FrameData[2] = _isonpower;
		m_CommandTimer.start(m_CommandGap);
	}
}

void CPwrCmdFrameMgr::_ParseFrame(UInt8* const _recvframe)
{
	//第8-9字节数据：上电状态
	for (int i = 0; i < 8; ++i) {
		m_PowerState[i] = _recvframe[7] % 2;
		_recvframe[7] >>= 1;
	}
	for (int i = 0; i < 2; ++i) {
		m_PowerState[i + 8] = _recvframe[8] % 2;
		_recvframe[8] >>= 1;
	}
}
