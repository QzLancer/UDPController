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
		m_PDUStruct.DA = 0x10 + _id - 1; //����������1
		//��1-2�ֽ����ݣ��豸ѡ����������������û�в��ԣ�
		m_PDUStruct.FrameData[0] = 0;
		m_PDUStruct.FrameData[1] = 0;
		for (int i = 0; i < 8; ++i) {
			m_PDUStruct.FrameData[0] += _deviceselect[i] * pow(2, i);
		}
		for (int i = 0; i < 2; ++i) {
			m_PDUStruct.FrameData[1] += _deviceselect[i + 8] * pow(2, i);
		}
		//��3�ֽ����ݣ��ϵ����
		m_PDUStruct.FrameData[2] = _isonpower;
		m_CommandTimer.setInterval(m_CommandGap);
		m_CommandTimer.start();
	}
}

void CPwrCmdFrameMgr::_ParseFrame(UInt8* const _recvframe)
{
	//��8-9�ֽ����ݣ��ϵ�״̬
	for (int i = 0; i < 8; ++i) {
		m_PowerStatus[i] = _recvframe[7] % 2;
		_recvframe[7] >>= 1;
	}
	for (int i = 0; i < 2; ++i) {
		m_PowerStatus[i + 8] = _recvframe[8] % 2;
		_recvframe[8] >>= 1;
	}
}
