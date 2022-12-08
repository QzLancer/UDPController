// 100ms����һ�������źţ�����������ʱ�̷����豸�Լ�֡

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
    /// @brief ���ӵ��豸
    void slotConnect();

    /// @brief ��ʼ�������
    void slotStartHeatBeat();

    /// @brief ֹͣ�������
    void slotStopHeatBeat();

    /// @brief �����������PDE
    void slotSendHeatBeatPDU();


protected:


    bool SendAllFrame::_ConvertPDUtoFrame(PDUStruct _pdu, UInt8* const _frame);

private:
    Ui::SendAllFrameClass ui;
    CUDPService m_UDPService;
    QTimer m_HeatBeatTimer;

    const int HeatBeatGap = 1000; //1000ms����һ����������Timer
};
