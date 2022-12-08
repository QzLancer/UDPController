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
    void slotConnect();

    /// @brief 开始心跳监测
    void slotStartHeatBeat();

    /// @brief 停止心跳监测
    void slotStopHeatBeat();

    /// @brief 发送心跳监测PDE
    void slotSendHeatBeatPDU();


protected:


    bool SendAllFrame::_ConvertPDUtoFrame(PDUStruct _pdu, UInt8* const _frame);

private:
    Ui::SendAllFrameClass ui;
    CUDPService m_UDPService;
    QTimer m_HeatBeatTimer;

    const int HeatBeatGap = 1000; //1000ms发送一次心跳监测的Timer
};
