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
    void slotClickConnectBtn();

    /// @brief ��ʼ�������
    void slotClickStartHeatBeatBtn();

    /// @brief ֹͣ�������
    void slotClickStopHeatBeatBtn();

    /// @brief ��ʼ�豸�Լ�;
    void slotClickSelfCheckBtn();

protected:


    bool SendAllFrame::_ConvertPDUtoFrame(PDUStruct _pdu, UInt8* const _frame);

private:
    Ui::SendAllFrameClass ui;

    CUDPService m_UDPService;

    QTimer m_HeatBeatTimer; //������ⶨʱ��
    const int m_HeatBeatGap = 1000; //1000ms����һ����������Timer
    
    int m_CommandCount = 0; //������ָ��ʹ���ͳ�ƣ�������3������Ӧ���ϱ�ͨѶ����
    QTimer m_CommandTimer;  //������ָ�ʱ����������ָ������Ӧ�������ʱ500ms������ָ��
    const int m_CommandGap = 500;   //��������500ms��������һ��ָ��

private slots:

    /// @brief �����������PDU
    void __slotSendHeatBeatPDU();

    /// @brief �����Լ�PDU
    void __slotSendSelfCheckPDU();
};
