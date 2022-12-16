#include "CController.h"

CController::CController(QWidget* parent)
	: QWidget(parent),
	m_UDPService(std::make_shared<CUDPService>()),
	m_SyncManager(m_UDPService),
	m_SelfCheckManager(m_UDPService),
	m_CommManager(m_UDPService),
	m_PwrManager(m_UDPService),
	m_RuntimeManager(m_UDPService),
	m_MessageManager(m_UDPService),
	m_ResetManager(m_UDPService)
{
	ui.setupUi(this);

	connect(ui.connectButton, SIGNAL(clicked()), this, SLOT(slotClickConnectBtn()));

	// 心跳检测频率为1Hz
	connect(ui.startButton, SIGNAL(clicked()), this, SLOT(slotClickStartHeartBeatBtn()));
	connect(ui.stopButton, SIGNAL(clicked()), this, SLOT(slotClickStopHeartBeatBtn()));

	// 发送设备自检帧
	connect(ui.selfcheckButton, SIGNAL(clicked()), this, SLOT(slotClickSelfCheckBtn()));

	// 发送通信控制帧
	connect(ui.commctrlButton, SIGNAL(clicked()), this, SLOT(slotClickCommCtrlBtn()));

	// 发送上电控制帧
	connect(ui.powerctrlButton, SIGNAL(clicked()), this, SLOT(slotClickPwrCtrlBtn()));

	// 发送运行时间查询控制帧	
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

	m_UDPService->ConnectSocket();

	connect(&m_UDPService->m_QSocket, SIGNAL(readyRead()), this, SLOT(__slotRecvFrame()));
}

void CController::slotClickDisconnectBtn()
{
	m_UDPService->DisconncetSocket();
	disconnect(&m_UDPService->m_QSocket, SIGNAL(readyRead()), this, SLOT(__slotRecvFrame()));
}

void CController::slotClickStartHeartBeatBtn()
{
	if (m_UDPService->ConnectStatus()) {
		m_SyncManager.StartHeartBeat(m_ControllerID);
	}
	else {
		printf("Message: This Controller is not connecting to host.");
	}
	
}

void CController::slotClickStopHeartBeatBtn()
{
	if (m_UDPService->ConnectStatus()) {
		m_SyncManager.StopHeartBeat(m_ControllerID);
	}
	else {
		printf("Message: This Controller is not connecting to host.");
	}
}

void CController::slotClickSelfCheckBtn()
{
	if (m_UDPService->ConnectStatus()) {
		m_SelfCheckManager.ExecCommand(m_ControllerID);
	}
	else {
		printf("Message: This Controller is not connecting to host.");
	}
}

void CController::slotClickCommCtrlBtn()
{
	if (m_UDPService->ConnectStatus()) {
		m_CommManager.ExecCommand(m_ControllerID, 1);
	}
	else {
		printf("Message: This Controller is not connecting to host.");
	}
}

void CController::slotClickPwrCtrlBtn()
{
	bool tmp[10] = {true, true, true, true, false, false , false , false , false , false };
	if (m_UDPService->ConnectStatus()) {
		m_PwrManager.ExecCommand(m_ControllerID, tmp, true);
	}
	else {
		printf("Message: This Controller is not connecting to host.");
	}
}

void CController::slotClickGetRuntimeBtn()
{
	if (m_UDPService->ConnectStatus()) {
		m_RuntimeManager.ExecCommand(m_ControllerID);
	}
	else {
		printf("Message: This Controller is not connecting to host.");
	}
}

void CController::__slotRecvFrame()
{
	if (m_UDPService->m_QSocket.pendingDatagramSize() <= 7) {
		printf("Message: Pending data size is less than 7.");
		return;
	}
	UInt8* recvdata = new UInt8[m_UDPService->m_QSocket.pendingDatagramSize()];
	m_UDPService->m_QSocket.readDatagram((char*)recvdata, 256);

	/// @brief 接收到心跳监测应答
	if (recvdata[5] == 0x04 && recvdata[6] == 0x00) {
		m_SyncManager._ParseFrame(recvdata);
	}
	/// @brief 接收到自检应答
	else if (recvdata[5] == 0x02 && recvdata[6] == 0x02) {
		m_SelfCheckManager.StopRecv();
		m_SelfCheckManager._ParseFrame(recvdata);
	}
	/// @brief 接收到通讯设置应答
	else if (recvdata[5] == 0x02 && recvdata[6] == 0x03) {
		m_CommManager.StopRecv();
		m_CommManager._ParseFrame(recvdata);
	}
	/// @brief 接收到控制状态应答
	else if (recvdata[5] == 0x02 && recvdata[6] == 0x05) {
		m_PwrManager.StopRecv();
		m_PwrManager._ParseFrame(recvdata);
	}
	/// @brief 接收到运行时间
	else if (recvdata[5] == 0x02 && recvdata[6] == 0x06) {
		m_RuntimeManager.StopRecv();
		m_RuntimeManager._ParseFrame(recvdata);
	}
	/// @brief 接收到PHM
	else if (recvdata[5] == 0x03 && recvdata[6] == 0x01) {
		m_MessageManager._ParseFrame(recvdata);
	}
	/// @brief 接收到复位应答
	else if (recvdata[5] == 0x02 && recvdata[6] == 0xFF) {
		m_ResetManager.StopRecv();
		m_ResetManager._ParseFrame(recvdata);
	}

	delete[] recvdata;
}

