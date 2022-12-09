#include "CController.h"

CController::CController(QWidget* parent)
	: QWidget(parent),
	m_UDPService(std::make_shared<CUDPService>()),
	m_SyncManager(m_UDPService),
	m_CommandFrameManager(m_UDPService)
{
	ui.setupUi(this);

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(slotClickConnectBtn()));

	// 心跳检测频率为1Hz
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(slotClickStartHeartBeatBtn()));
	connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(slotClickStopHeartBeatBtn()));

	// 发送设备自检帧
	connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(slotClickSelfCheckBtn()));
}

CController::~CController()
{}

void CController::slotClickConnectBtn()
{
	m_UDPService->SetHostIP("192.168.199.235");
	m_UDPService->SetInitHostPort(60100 + m_ControllerID - 1);
	m_UDPService->SetControllerIP("192.168.199.235");
	m_UDPService->SetInitControllerPort(60000 + m_ControllerID - 1);

	m_UDPService->ConnectSocket(m_ControllerID);
}

void CController::slotClickDisconnectBtn()
{
	m_UDPService->DisconncetSocket(m_ControllerID);
}

void CController::slotClickStartHeartBeatBtn()
{
	if (m_UDPService->ConnectStatus(m_ControllerID)) {
		m_SyncManager.StartHeartBeat(m_ControllerID);
	}
	else {
		printf("Message: This Controller is not connecting to host.");
	}
	
}

void CController::slotClickStopHeartBeatBtn()
{
	if (m_UDPService->ConnectStatus(m_ControllerID)) {
		m_SyncManager.StopHeartBeat(m_ControllerID);
	}
	else {
		printf("Message: This Controller is not connecting to host.");
	}
}

void CController::slotClickSelfCheckBtn()
{
	if (m_UDPService->ConnectStatus(m_ControllerID)) {
		m_CommandFrameManager.SelfCheck(m_ControllerID);
	}
	else {
		printf("Message: This Controller is not connecting to host.");
	}
}

