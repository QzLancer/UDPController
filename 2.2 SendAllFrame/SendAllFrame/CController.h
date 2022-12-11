// 100ms发送一次心跳信号，并且在任意时刻发送设备自检帧

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

/// @brief 控制器类，管理单个控制器的UDP通信、数据帧的发送与接收
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

    /// @brief 开始通信控制
    void slotClickCommCtrlBtn();

    /// @brief 开始上电控制
    void slotClickPwrCtrlBtn();

    /// @brief 获取运行事件
    void slotClickGetRuntimeBtn();

private:
    Ui::CControllerClass ui;

    std::shared_ptr<CUDPService> m_UDPService;

    CHbFrameMgr m_SyncManager;
    
    CSCCmdFrameMgr m_CSCManager;

    CCommCmdFrameMgr m_CCommManager;

    CPwrCmdFrameMgr m_CPwrManager;

    CRuntimeCmdFrameMgr m_CRuntimeManager;

    int m_ControllerID = 1; //控制器id，取值为1-15
};
