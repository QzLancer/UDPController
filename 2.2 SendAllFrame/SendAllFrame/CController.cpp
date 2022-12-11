#include "CController.h"

CController::CController(QWidget* parent)
	: QWidget(parent),
	m_UDPService(std::make_shared<CUDPService>()),
	m_SyncManager(m_UDPService),
	m_CSCManager(m_UDPService),
	m_CCommManager(m_UDPService),
	m_CPwrManager(m_UDPService),
	m_CRuntimeManager(m_UDPService)
{
	ui.setupUi(this);

	connect(ui.connectButton, SIGNAL(clicked()), this, SLOT(slotClickConnectBtn()));

	// �������Ƶ��Ϊ1Hz
	connect(ui.startButton, SIGNAL(clicked()), this, SLOT(slotClickStartHeartBeatBtn()));
	connect(ui.stopButton, SIGNAL(clicked()), this, SLOT(slotClickStopHeartBeatBtn()));

	// �����豸�Լ�֡
	connect(ui.selfcheckButton, SIGNAL(clicked()), this, SLOT(slotClickSelfCheckBtn()));

	// ����ͨ�ſ���֡
	connect(ui.commctrlButton, SIGNAL(clicked()), this, SLOT(slotClickCommCtrlBtn()));

	// �����ϵ����֡
	connect(ui.powerctrlButton, SIGNAL(clicked()), this, SLOT(slotClickPwrCtrlBtn()));

	// ��������ʱ���ѯ����֡	
	connect(ui.gettimeButton, SIGNAL(clicked()), this, SLOT(slotClickGetRuntimeBtn()));
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
		m_CSCManager.ExecCommand(m_ControllerID);
	}
	else {
		printf("Message: This Controller is not connecting to host.");
	}
}

void CController::slotClickCommCtrlBtn()
{
	if (m_UDPService->ConnectStatus(m_ControllerID)) {
		m_CCommManager.ExecCommand(m_ControllerID, 1);
	}
	else {
		printf("Message: This Controller is not connecting to host.");
	}
}

void CController::slotClickPwrCtrlBtn()
{
	bool tmp[10] = {true, true, true, true, false, false , false , false , false , false };
	if (m_UDPService->ConnectStatus(m_ControllerID)) {
		m_CPwrManager.ExecCommand(m_ControllerID, tmp, true);
	}
	else {
		printf("Message: This Controller is not connecting to host.");
	}
}

void CController::slotClickGetRuntimeBtn()
{
	if (m_UDPService->ConnectStatus(m_ControllerID)) {
		m_CRuntimeManager.ExecCommand(m_ControllerID);
	}
	else {
		printf("Message: This Controller is not connecting to host.");
	}
}

