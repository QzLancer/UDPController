#include "SendAllFrame.h"

SendAllFrame::SendAllFrame(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(slotClickConnectBtn()));

	// 心跳检测频率为1Hz
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(slotClickStartHeatBeatBtn()));
	connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(slotClickStopHeatBeatBtn()));
	connect(&m_HeatBeatTimer, SIGNAL(timeout()), this, SLOT(__slotSendHeatBeatPDU()));

	// 发送设备自检帧
	connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(slotClickSelfCheckBtn()));
	connect(&m_CommandTimer, SIGNAL(timeout()), this, SLOT(__slotSendSelfCheckPDU()));
}

SendAllFrame::~SendAllFrame()
{}

void SendAllFrame::slotClickConnectBtn()
{
	m_UDPService.SetHostIP("192.168.199.235");
	m_UDPService.SetInitHostPort(60100);
	m_UDPService.SetControllerIP("192.168.199.235");
	m_UDPService.SetInitControllerPort(60000);

	m_UDPService.ConnectSocket(1);


}

void SendAllFrame::slotClickStartHeatBeatBtn()
{
	if (!m_HeatBeatTimer.isActive()) {
		m_HeatBeatTimer.start(m_HeatBeatGap);
	}
}

void SendAllFrame::slotClickStopHeatBeatBtn()
{
	if (m_HeatBeatTimer.isActive()) {
		m_HeatBeatTimer.stop();
	}
}

void SendAllFrame::slotClickSelfCheckBtn()
{
	if (!m_CommandTimer.isActive()) {
		m_CommandTimer.start(m_CommandGap);
	}
}

bool SendAllFrame::_ConvertPDUtoFrame(PDUStruct _pdu, UInt8* const _frame)
{
	if (_pdu.N < 7) {
		printf("Error: Frame size is less than 7!");
		return false;
	}
	_frame[0] = _pdu.Head1;
	_frame[1] = _pdu.Head2;
	_frame[2] = _pdu.N;
	_frame[3] = _pdu.SA;
	_frame[4] = _pdu.DA;
	_frame[5] = _pdu.PF1;
	_frame[6] = _pdu.PF2;
	for (UInt8 i = 7; i < _pdu.N; ++i) {
		_frame[i] = _pdu.FrameData[i - 7];
	}

	return true;
}

void SendAllFrame::__slotSendHeatBeatPDU()
{
	UInt8 send[256] = { "HeatBeat PDU" };
	m_UDPService.SendFrame(send, 14, 1);
}

void SendAllFrame::__slotSendSelfCheckPDU()
{
	PDUStruct SelfCheckPDU;
	SelfCheckPDU.N = 15;
	SelfCheckPDU.SA = 0x01;
	SelfCheckPDU.DA = 0x10; //控制器代号1
	SelfCheckPDU.PF1 = 0x01;
	SelfCheckPDU.PF2 = 0x02;
	for (int i = 0; i < 8; ++i) {
		SelfCheckPDU.FrameData[i] = 0x00;
	}

	UInt8 sendframe[15];
	UInt8 recvframe[22];
	_ConvertPDUtoFrame(SelfCheckPDU, sendframe);

	if (m_UDPService.SendFrame(sendframe, 15, 1)) {
		if (m_UDPService.RecvFrame(recvframe, 22, 1)) {
			printf("Message: Received self check response.\n");
			printf("frame: ");
			for (int i = 0; i < 15; ++i) {
				printf("%#X ", recvframe[i]);
			}
			printf("\n");
			m_CommandCount = 0;
			if (m_CommandTimer.isActive()) {
				m_CommandTimer.stop();
			}
		}
		else {
			m_CommandCount++;
			printf("Message: The %d try is failied, wait 500ms for next try.\n", m_CommandCount);
			
			if (m_CommandCount >= 3) {
				printf("Message: Self check failed.\n");
				m_CommandCount = 0;
				m_CommandCount = 0;
				if (m_CommandTimer.isActive()) {
					m_CommandTimer.stop();
				}
			}
		}
	}
}
