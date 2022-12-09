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

    int m_CommandCount = 0; //命令类指令发送次数统计，若发送3次无响应，上报通讯故障
    QTimer m_CommandTimer;  //命令类指令定时器，用于在指令无响应情况下延时500ms发送新指令
    const int m_CommandGap = 500;   //触发后间隔500ms主动发送一次指令

private slots:
    /// @brief 发送自检PDU
    void __slotSendSelfCheckPDU();
};

