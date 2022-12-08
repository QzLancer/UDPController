#include "SendAllFrame.h"

SendAllFrame::SendAllFrame(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(slotConnet()));
    connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(slotConnet2()));
}

SendAllFrame::~SendAllFrame()
{}

void SendAllFrame::slotConnet()
{
    m_udpservice.SetHostIP("192.168.199.235");
    m_udpservice.SetInitHostPort(60100);
    m_udpservice.SetControllerIP("192.168.199.235");
    m_udpservice.SetInitControllerPort(60000);

    m_udpservice.ConnectSocket(1);

    UInt8 sendbuf[256] = { "Hello Controller 1!" };

    m_udpservice.SendFrame(sendbuf, 20, 1);

    m_udpservice.DisconncetSocket(2);
}

void SendAllFrame::slotConnet2()
{
    m_udpservice.ConnectSocket(2);

    UInt8 sendbuf2[256] = { "Hello Controller 2!" };

    m_udpservice.SendFrame(sendbuf2, 20, 2);

    UInt8 sendbuf[256] = { "Hello Controller 1!" };

    m_udpservice.SendFrame(sendbuf, 20, 1);
}
