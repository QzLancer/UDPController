// 100ms发送一次心跳信号，并且在任意时刻发送设备自检帧

#pragma once

#include <QtWidgets/QWidget>
#include <qtimer.h>

#include "ui_SendAllFrame.h"
#include "CUDPService.h"
#include "datatype.h"

class SendAllFrame : public QWidget
{
    Q_OBJECT

public:
    SendAllFrame(QWidget *parent = nullptr);
    ~SendAllFrame();

public slots:
    /// @brief 连接到设备
    void slotClickConnectBtn();

    /// @brief 开始心跳监测
    void slotClickStartHeatBeatBtn();

    /// @brief 停止心跳监测
    void slotClickStopHeatBeatBtn();

    /// @brief 开始设备自检;
    void slotClickSelfCheckBtn();

protected:


    bool SendAllFrame::_ConvertPDUtoFrame(PDUStruct _pdu, UInt8* const _frame);

private:
    Ui::SendAllFrameClass ui;

    CUDPService m_UDPService;

    QTimer m_HeatBeatTimer; //心跳监测定时器
    const int m_HeatBeatGap = 1000; //1000ms发送一次心跳监测的Timer
    
    int m_CommandCount = 0; //命令类指令发送次数统计，若发送3次无响应，上报通讯故障
    QTimer m_CommandTimer;  //命令类指令定时器，用于在指令无响应情况下延时500ms发送新指令
    const int m_CommandGap = 500;   //触发后间隔500ms主动发送一次指令

private slots:

    /// @brief 发送心跳监测PDU
    void __slotSendHeatBeatPDU();

    /// @brief 发送自检PDU
    void __slotSendSelfCheckPDU();
};
