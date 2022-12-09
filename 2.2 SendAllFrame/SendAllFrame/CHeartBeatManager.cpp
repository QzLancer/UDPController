#include "CHeartBeatManager.h"

CHeartBeatManager::CHeartBeatManager(std::shared_ptr<CUDPService> _udpservice):
	m_UDPService(_udpservice)
{
	m_HeartBeatPDU.SA = 0x01;
	m_HeartBeatPDU.Head1 = 0xA5;
	m_HeartBeatPDU.Head2 = 0x5A;
	m_HeartBeatPDU.N = 15;
	m_HeartBeatPDU.PF1 = 0x00;
	m_HeartBeatPDU.PF2 = 0x00;
	for (int i = 0; i < 8; ++i) {
		m_HeartBeatPDU.FrameData[i] = {0x00};
	}
	connect(&m_HeartBeatTimer, SIGNAL(timeout()), this, SLOT(__slotSendHeartBeatPDU()));

}

void CHeartBeatManager::StartHeartBeat(int _id)
{
	m_ControllerID = _id;

	m_HeartBeatPDU.DA = 0x10 + _id - 1;

	if (!m_HeartBeatTimer.isActive()) {
		m_HeartBeatTimer.start(m_HeartBeatGap);
	}
}

void CHeartBeatManager::StopHeartBeat(int _id)
{
	if (m_HeartBeatTimer.isActive()) {
		m_HeartBeatTimer.stop();
	}
}

void CHeartBeatManager::__slotSendHeartBeatPDU()
{
	// ��ȡ��Ϣ����ʱ�䣬���32λ���ݱ���ms����ʱ�������ౣ��23�죬������﷢�͵�ʱ�䶨��Ϊ����ǰ��ʱ/��/��/����ת���ɺ���ʱ�������
	QTime time = QTime::currentTime();
	int timestick = time.msec();
	timestick += time.second() * 1000;
	timestick += time.minute() * 60 * 1000;
	timestick += time.hour() * 60 * 60 * 1000;

	// ����PDU
	for (int i = 0; i < 3; ++i) {
		m_HeartBeatPDU.FrameData[i] = (timestick % 256);
		timestick /= 256;
	}

	// ��PDUת����֡
	UInt8 frame[15] = { 0 };
	m_UDPService->ConvertPDUtoFrame(m_HeartBeatPDU, frame);

	// ����֡
	m_UDPService->SendFrame(frame, 15, 1);
}
