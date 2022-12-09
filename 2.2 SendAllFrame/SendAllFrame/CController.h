// 100ms发送一次心跳信号，并且在任意时刻发送设备自检帧

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
    /// @brief 连接到设备
    void slotClickConnectBtn();

    /// @brief 断开和设备的连接
    void slotClickDisconnectBtn();

    /// @brief 开始心跳监测
    void slotClickStartHeartBeatBtn();

    /// @brief 停止心跳监测
    void slotClickStopHeartBeatBtn();

    /// @brief 开始设备自检;
    void slotClickSelfCheckBtn();


private:
    Ui::SendAllFrameClass ui;

    std::shared_ptr<CUDPService> m_UDPService;

    CHeartBeatManager m_SyncManager;
    
    CCommandFrameManager m_CommandFrameManager;

    int m_ControllerID = 1; //控制器id，取值为1-15
};
