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
		m_PDUStruct.DA = 0x10 + _id - 1; //����������1
		for (int i = 0; i < 8; ++i) {
			m_PDUStruct.FrameData[i] = 0x00;
		}
		m_CommandTimer.start(m_CommandGap);
	}
}

void CSCCmdFrameMgr::_ParseFrame(UInt8* const _recvframe)
{
	// ��8�ֽ����ݣ��ӵ����״̬
	for (int i = 0; i < 5; ++i) {
		m_PowerState[i] = _recvframe[7] % 2;
		_recvframe[7] >>= 1;
	}

	// ��9-10�ֽ����ݣ���ѹ�쳣
	for (int i = 0; i < 8; ++i) {
		m_OverVoltage[i] = _recvframe[8] % 2;
		_recvframe[8] >>= 1;
	}
	for (int i = 0; i < 2; ++i) {
		m_OverVoltage[i + 8] = _recvframe[9] % 2;
		_recvframe[9] >>= 1;
	}

	// ��11-12�ֽ����ݣ�Ƿѹ�쳣
	for (int i = 0; i < 8; ++i) {
		m_UnderVoltage[i] = _recvframe[10] % 2;
		_recvframe[10] >>= 1;
	}
	for (int i = 0; i < 2; ++i) {
		m_UnderVoltage[i + 8] = _recvframe[11] % 2;
		_recvframe[11] >>= 1;
	}

	// ��13-14�ֽ����ݣ������쳣
	for (int i = 0; i < 8; ++i) {
		m_OverCurrent[i] = _recvframe[12] % 2;
		_recvframe[12] >>= 1;
	}
	for (int i = 0; i < 2; ++i) {
		m_OverCurrent[i + 8] = _recvframe[13] % 2;
		_recvframe[13] >>= 1;
	}

	// ��16-17�ֽ����ݣ������쳣
	for (int i = 0; i < 8; ++i) {
		m_OverTemp[i] = _recvframe[14] % 2;
		_recvframe[15] >>= 1;
	}
	for (int i = 0; i < 2; ++i) {
		m_OverTemp[i + 8] = _recvframe[15] % 2;
		_recvframe[16] >>= 1;
	}

	// ��18-21�ֽ����ݣ�������ʱ���Է���Ϊ��λ
	m_TotalStartupTime = _recvframe[17];
	m_TotalStartupTime += _recvframe[18] * 256;
	m_TotalStartupTime += _recvframe[19] * 256 * 256;
	m_TotalStartupTime += _recvframe[20] * 256 * 256 * 256;
}
