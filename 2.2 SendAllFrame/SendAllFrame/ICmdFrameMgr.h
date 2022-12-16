#pragma once

#include "IFrameMgr.h"
#include <QtCore/QTimer>

/// @brief ������֡�͸�λ֡�������
class ICmdFrameMgr :
    public IFrameMgr
{
    Q_OBJECT

public:
    explicit ICmdFrameMgr(std::shared_ptr<CUDPService> _udpservice);

    void StopRecv();

protected:

    int m_CommandCount = 0; //������ָ��ʹ���ͳ�ƣ�������3������Ӧ���ϱ�ͨѶ����
    QTimer m_CommandTimer;  //������ָ�ʱ����������ָ������Ӧ�������ʱ500ms������ָ��
    const int m_CommandGap = 500;   //��������500ms��������һ��ָ��
    QTimer m_RecvTimer; //����ָ�ʱ��������ָ���ʼ��ʱ���������100msδ���ܵ�ָ�����Ϊ����Ӧ��ִ�вۺ���
    const int m_RecvGap = 100;  //����ָ��ȴ�100ms

private slots:
    /// @brief �����Լ�PDU
    virtual void __slotSendPDU();
    virtual void __slotTimeout();
};

