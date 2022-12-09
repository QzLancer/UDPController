// 100ms����һ�������źţ�����������ʱ�̷����豸�Լ�֡

#pragma once

#include "ui_SendAllFrame.h"
#include "CUDPService.h"
#include "datatype.h"
#include "CHeartBeatManager.h"
#include "CCommandFrameManager.h"
#include <QtWidgets/QWidget>
#include <QtCore/QTimer>

class CController : public QWidget
{
    Q_OBJECT

public:
    CController(QWidget *parent = nullptr);
    ~CController();

public slots:
    /// @brief ���ӵ��豸
    void slotClickConnectBtn();

    /// @brief �Ͽ����豸������
    void slotClickDisconnectBtn();

    /// @brief ��ʼ�������
    void slotClickStartHeartBeatBtn();

    /// @brief ֹͣ�������
    void slotClickStopHeartBeatBtn();

    /// @brief ��ʼ�豸�Լ�;
    void slotClickSelfCheckBtn();


private:
    Ui::SendAllFrameClass ui;

    std::shared_ptr<CUDPService> m_UDPService;

    CHeartBeatManager m_SyncManager;
    
    CCommandFrameManager m_CommandFrameManager;

    int m_ControllerID = 1; //������id��ȡֵΪ1-15
};
