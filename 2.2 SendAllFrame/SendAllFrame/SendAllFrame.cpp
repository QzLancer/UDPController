#include "SendAllFrame.h"

SendAllFrame::SendAllFrame(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(slotConnect()));

    // ÐÄÌø¼ì²âÆµÂÊÎª1Hz
    connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(slotStartHeatBeat()));
    connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(slotStopHeatBeat()));
    connect(&m_HeatBeatTimer, SIGNAL(timeout()), this, SLOT(slotSendHeatBeatPDU()));
}

SendAllFrame::~SendAllFrame()
{}

void SendAllFrame::slotConnect()
{
    m_UDPService.SetHostIP("192.168.199.235");
    m_UDPService.SetInitHostPort(60100);
    m_UDPService.SetControllerIP("192.168.199.235");
    m_UDPService.SetInitControllerPort(60000);

    m_UDPService.ConnectSocket(1);

    
}

void SendAllFrame::slotStartHeatBeat()
{
    if (!m_HeatBeatTimer.isActive()) {
        m_HeatBeatTimer.start(HeatBeatGap);
    }
}

void SendAllFrame::slotStopHeatBeat()
{
    if (m_HeatBeatTimer.isActive()) {
        m_HeatBeatTimer.stop();
    }
}

void SendAllFrame::slotSendHeatBeatPDU()
{
    UInt8 send[256] = { "HeatBeat PDU" };
    m_UDPService.SendFrame(send, 14, 1);
}

bool SendAllFrame::_ConvertPDUtoFrame(PDUStruct _pdu, UInt8* const _frame)
{
    if (_pdu.N < 7) {
        printf("Error: Frame size is less than 7!");
        return false;
    }
    UInt8* frame = new UInt8[_pdu.N];
    frame[0] = _pdu.Head1;
    frame[1] = _pdu.Head2;
    frame[2] = _pdu.N;
    frame[3] = _pdu.SA;
    frame[4] = _pdu.DA;
    frame[5] = _pdu.PF1;
    frame[6] = _pdu.PF2;
    for (UInt8 i = 7; i < _pdu.N; ++i) {
        frame[i] = _pdu.FrameData[i - 7];
    }

    return true;
}
