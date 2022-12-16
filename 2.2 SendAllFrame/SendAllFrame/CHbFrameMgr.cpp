#include "CHbFrameMgr.h"

CHbFrameMgr::CHbFrameMgr(std::shared_ptr<CUDPService> _udpservice):
	IFrameMgr(_udpservice)
{
	m_SendFrameSize = 15;
	m_RecvFrameSize = 15;

	m_SendFrame = new UInt8[m_SendFrameSize];
	m_RecvFrame = new UInt8[m_RecvFrameSize];

	m_PDUStruct.N = 15;
	m_PDUStruct.PF1 = 0x00;
	m_PDUStruct.PF2 = 0x00;
	for (int i = 0; i < 8; ++i) {
		m_PDUStruct.FrameData[i] = {0x00};
	}
	connect(&m_HeartBeatTimer, SIGNAL(timeout()), this, SLOT(__slotSendHeartBeatPDU()));

}

void CHbFrameMgr::StartHeartBeat(int _id)
{
	m_ControllerID = _id;

	m_PDUStruct.DA = 0x10 + _id - 1;

	if (!m_HeartBeatTimer.isActive()) {
		m_HeartBeatTimer.start(m_HeartBeatGap);
	}
}

void CHbFrameMgr::StopHeartBeat(int _id)
{
	if (m_HeartBeatTimer.isActive()) {
		m_HeartBeatTimer.stop();
	}
}

void CHbFrameMgr::_ParseFrame(UInt8* const _recvframe)
{
	//��8-11�ֽ����ݣ���Ϣ����ʱ��
	//memcpy(&m_ReceivedTime, &_recvframe[7], 4 * sizeof(UInt8));
	m_ReceivedTime = 0;
	for (int i = 0; i < 4; ++i) {
		m_ReceivedTime += _recvframe[i + 7] * pow(256, i);
	}
	
	//��12-13�ֽ����ݣ��豸���µ�״̬
	for (int i = 0; i < 8; ++i) {
		m_PowerStatus[i] = _recvframe[11] % 2;
		_recvframe[11] >>= 1;
	}
	for (int i = 0; i < 2; ++i) {
		m_PowerStatus[i + 8] = _recvframe[12] % 2;
		_recvframe[12] >>= 1;
	}

	//��14�ֽ����ݣ����״̬
	CombineStatus = _recvframe[13];

}

void CHbFrameMgr::__slotSendHeartBeatPDU()
{
	// ��ȡ��Ϣ����ʱ�䣬���32λ���ݱ���ms����ʱ�������ౣ��23�죬������﷢�͵�ʱ�䶨��Ϊ����ǰ��ʱ/��/��/����ת���ɺ���ʱ�������
	QTime time = QTime::currentTime();
	int timestick = time.msec();
	timestick += time.second() * 1000;
	timestick += time.minute() * 60 * 1000;
	timestick += time.hour() * 60 * 60 * 1000;

	// ����PDU
	for (int i = 0; i < 4; ++i) {
		m_PDUStruct.FrameData[i] = (timestick % 256);
		timestick /= 256;
	}

	// ��PDUת����֡
	m_UDPService->ConvertPDUtoFrame(m_PDUStruct, m_SendFrame);

	// ����֡
	if (m_UDPService->SendFrame(m_SendFrame, 15, 1)) {
		//if (m_UDPService->RecvFrame(recvframe, 15, 1)) {
		//	_ParseFrame(recvframe);
		//}
	};
}
