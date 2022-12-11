#include "CSCCmdFrameMgr.h"

CSCCmdFrameMgr::CSCCmdFrameMgr(std::shared_ptr<CUDPService> _udpservice) : 
	ICmdFrameMgr(_udpservice)
{
	m_SendFrameSize = 15;
	m_RecvFrameSize = 22;

	m_SendFrame = new UInt8[m_SendFrameSize];
	m_RecvFrame = new UInt8[m_RecvFrameSize];

}

void CSCCmdFrameMgr::ExecCommand(int _id)
{
	if (!m_CommandTimer.isActive()) {
		m_PDUStruct.N = m_SendFrameSize;
		m_PDUStruct.PF1 = 0x01;
		m_PDUStruct.PF2 = 0x02;
		m_PDUStruct.DA = 0x10 + _id - 1; //控制器代号1
		for (int i = 0; i < 8; ++i) {
			m_PDUStruct.FrameData[i] = 0x00;
		}
		m_CommandTimer.start(m_CommandGap);
	}
}

void CSCCmdFrameMgr::_ParseFrame(UInt8* const _recvframe)
{
	// 第8字节数据：加电组合状态
	for (int i = 0; i < 5; ++i) {
		m_PowerState[i] = _recvframe[7] % 2;
		_recvframe[7] >>= 1;
	}

	// 第9-10字节数据：过压异常
	for (int i = 0; i < 8; ++i) {
		m_OverVoltage[i] = _recvframe[8] % 2;
		_recvframe[8] >>= 1;
	}
	for (int i = 0; i < 2; ++i) {
		m_OverVoltage[i + 8] = _recvframe[9] % 2;
		_recvframe[9] >>= 1;
	}

	// 第11-12字节数据：欠压异常
	for (int i = 0; i < 8; ++i) {
		m_UnderVoltage[i] = _recvframe[10] % 2;
		_recvframe[10] >>= 1;
	}
	for (int i = 0; i < 2; ++i) {
		m_UnderVoltage[i + 8] = _recvframe[11] % 2;
		_recvframe[11] >>= 1;
	}

	// 第13-14字节数据：过流异常
	for (int i = 0; i < 8; ++i) {
		m_OverCurrent[i] = _recvframe[12] % 2;
		_recvframe[12] >>= 1;
	}
	for (int i = 0; i < 2; ++i) {
		m_OverCurrent[i + 8] = _recvframe[13] % 2;
		_recvframe[13] >>= 1;
	}

	// 第16-17字节数据：过温异常
	for (int i = 0; i < 8; ++i) {
		m_OverTemp[i] = _recvframe[14] % 2;
		_recvframe[15] >>= 1;
	}
	for (int i = 0; i < 2; ++i) {
		m_OverTemp[i + 8] = _recvframe[15] % 2;
		_recvframe[16] >>= 1;
	}

	// 第18-21字节数据：开机总时长以分钟为单位
	m_TotalStartupTime = _recvframe[17];
	m_TotalStartupTime += _recvframe[18] * 256;
	m_TotalStartupTime += _recvframe[19] * 256 * 256;
	m_TotalStartupTime += _recvframe[20] * 256 * 256 * 256;
}
