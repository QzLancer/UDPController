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

    void StopRecv();

protected:

    int m_CommandCount = 0; //命令类指令发送次数统计，若发送3次无响应，上报通讯故障
    QTimer m_CommandTimer;  //命令类指令定时器，用于在指令无响应情况下延时500ms发送新指令
    const int m_CommandGap = 500;   //触发后间隔500ms主动发送一次指令
    QTimer m_RecvTimer; //接收指令定时器，发送指令后开始计时，如果超过100ms未接受到指令，则视为无响应，执行槽函数
    const int m_RecvGap = 100;  //单个指令等待100ms

private slots:
    /// @brief 发送自检PDU
    virtual void __slotSendPDU();
    virtual void __slotTimeout();
};

