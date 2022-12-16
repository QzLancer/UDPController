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
	//第8-11字节数据：信息产生时间
	//memcpy(&m_ReceivedTime, &_recvframe[7], 4 * sizeof(UInt8));
	m_ReceivedTime = 0;
	for (int i = 0; i < 4; ++i) {
		m_ReceivedTime += _recvframe[i + 7] * pow(256, i);
	}
	
	//第12-13字节数据：设备上下电状态
	for (int i = 0; i < 8; ++i) {
		m_PowerStatus[i] = _recvframe[11] % 2;
		_recvframe[11] >>= 1;
	}
	for (int i = 0; i < 2; ++i) {
		m_PowerStatus[i + 8] = _recvframe[12] % 2;
		_recvframe[12] >>= 1;
	}

	//第14字节数据：组合状态
	CombineStatus = _recvframe[13];

}

void CHbFrameMgr::__slotSendHeartBeatPDU()
{
	// 获取信息产生时间，如果32位数据保存ms级别时间戳，最多保留23天，因此这里发送的时间定义为将当前的时/分/秒/毫秒转换成毫秒时间戳发送
	QTime time = QTime::currentTime();
	int timestick = time.msec();
	timestick += time.second() * 1000;
	timestick += time.minute() * 60 * 1000;
	timestick += time.hour() * 60 * 60 * 1000;

	// 整理PDU
	for (int i = 0; i < 4; ++i) {
		m_PDUStruct.FrameData[i] = (timestick % 256);
		timestick /= 256;
	}

	// 将PDU转换成帧
	m_UDPService->ConvertPDUtoFrame(m_PDUStruct, m_SendFrame);

	// 发送帧
	if (m_UDPService->SendFrame(m_SendFrame, 15, 1)) {
		//if (m_UDPService->RecvFrame(recvframe, 15, 1)) {
		//	_ParseFrame(recvframe);
		//}
	};
}
