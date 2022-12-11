// 100ms����һ�������źţ�����������ʱ�̷����豸�Լ�֡

#pragma once

#include "ui_CController.h"
#include "CUDPService.h"
#include "datatype.h"
#include "CHbFrameMgr.h"
#include "CSCCmdFrameMgr.h"
#include "CCommCmdFrameMgr.h"
#include "CPwrCmdFrameMgr.h"
#include "CRuntimeCmdFrameMgr.h"

#include <QtWidgets/QWidget>
#include <QtCore/QTimer>

/// @brief �������࣬��������������UDPͨ�š�����֡�ķ��������
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

    /// @brief ��ʼͨ�ſ���
    void slotClickCommCtrlBtn();

    /// @brief ��ʼ�ϵ����
    void slotClickPwrCtrlBtn();

    /// @brief ��ȡ�����¼�
    void slotClickGetRuntimeBtn();

private:
    Ui::CControllerClass ui;

    std::shared_ptr<CUDPService> m_UDPService;

    CHbFrameMgr m_SyncManager;
    
    CSCCmdFrameMgr m_CSCManager;

    CCommCmdFrameMgr m_CCommManager;

    CPwrCmdFrameMgr m_CPwrManager;

    CRuntimeCmdFrameMgr m_CRuntimeManager;

    int m_ControllerID = 1; //������id��ȡֵΪ1-15
};
