#pragma once

#include "IFrameMgr.h"
#include <QtCore/QTimer>

class ICmdFrameMgr :
    public IFrameMgr
{
    Q_OBJECT

public:
    explicit ICmdFrameMgr(std::shared_ptr<CUDPService> _udpservice);

    virtual ~ICmdFrameMgr();

protected:

    int m_CommandCount = 0; //������ָ��ʹ���ͳ�ƣ�������3������Ӧ���ϱ�ͨѶ����
    QTimer m_CommandTimer;  //������ָ�ʱ����������ָ������Ӧ�������ʱ500ms������ָ��
    const int m_CommandGap = 500;   //��������500ms��������һ��ָ��

    int m_SendFrameSize = 0;
    int m_RecvFrameSize = 0;

    UInt8* m_SendFrame = NULL;
    UInt8* m_RecvFrame = NULL;

private slots:
    /// @brief �����Լ�PDU
    virtual void __slotSendPDU();
};

