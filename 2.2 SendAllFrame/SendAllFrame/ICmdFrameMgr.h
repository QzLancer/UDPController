#pragma once

#include "IFrameMgr.h"
#include <QtCore/QTimer>

/// @brief 命令类帧和复位帧抽象基类
class ICmdFrameMgr :
    public IFrameMgr
{
    Q_OBJECT

public:
    explicit ICmdFrameMgr(std::shared_ptr<CUDPService> _udpservice);

protected:

    int m_CommandCount = 0; //命令类指令发送次数统计，若发送3次无响应，上报通讯故障
    QTimer m_CommandTimer;  //命令类指令定时器，用于在指令无响应情况下延时500ms发送新指令
    const int m_CommandGap = 500;   //触发后间隔500ms主动发送一次指令

private slots:
    /// @brief 发送自检PDU
    virtual void __slotSendPDU();
};

