#pragma once

#include "datatype.h"
#include "CUDPService.h"
#include <QObject>
#include <QtCore/QTimer>

class CCommandFrameManager :
    public QObject
{
    Q_OBJECT

public:
    explicit CCommandFrameManager(std::shared_ptr<CUDPService> _udpservice);

    void SelfCheck(int _id);
private:

    std::shared_ptr<CUDPService> m_UDPService;

    PDUStruct m_SelfCheckPDU;

    int m_CommandCount = 0; //������ָ��ʹ���ͳ�ƣ�������3������Ӧ���ϱ�ͨѶ����
    QTimer m_CommandTimer;  //������ָ�ʱ����������ָ������Ӧ�������ʱ500ms������ָ��
    const int m_CommandGap = 500;   //��������500ms��������һ��ָ��

private slots:
    /// @brief �����Լ�PDU
    void __slotSendSelfCheckPDU();
};

