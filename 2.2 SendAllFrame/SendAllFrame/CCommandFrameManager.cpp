#include "CCommandFrameManager.h"

CCommandFrameManager::CCommandFrameManager(std::shared_ptr<CUDPService> _udpservice):
	m_UDPService(_udpservice)
{
	m_SelfCheckPDU.SA = 0x01;
	m_SelfCheckPDU.Head1 = 0xA5;
	m_SelfCheckPDU.Head2 = 0x5A;
	m_SelfCheckPDU.N = 15;
	m_SelfCheckPDU.PF1 = 0x01;
	m_SelfCheckPDU.PF2 = 0x02;
	for (int i = 0; i < 8; ++i) {
		m_SelfCheckPDU.FrameData[i] = 0x00;
	}

	connect(&m_CommandTimer, SIGNAL(timeout()), this, SLOT(__slotSendSelfCheckPDU()));
}

void CCommandFrameManager::SelfCheck(int _id)
{
	if (!m_CommandTimer.isActive()) {
		m_SelfCheckPDU.DA = 0x10 + _id - 1; //¿ØÖÆÆ÷´úºÅ1
		m_CommandTimer.start(m_CommandGap);
	}
}

void CCommandFrameManager::__slotSendSelfCheckPDU()
{
	UInt8 sendframe[15];
	UInt8 recvframe[22];
	m_UDPService->ConvertPDUtoFrame(m_SelfCheckPDU, sendframe);

	if (m_UDPService->SendFrame(sendframe, 15, 1)) {
		if (m_UDPService->RecvFrame(recvframe, 22, 1)) {
			printf("Message: Received self check response.\n");
			printf("frame: ");
			for (int i = 0; i < 15; ++i) {
				printf("%#X ", recvframe[i]);
			}
			printf("\n");
			m_CommandCount = 0;
			if (m_CommandTimer.isActive()) {
				m_CommandTimer.stop();
			}
		}
		else {
			m_CommandCount++;
			printf("Message: The %d try is failied, wait 500ms for next try.\n", m_CommandCount);

			if (m_CommandCount >= 3) {
				printf("Message: Self check failed.\n");
				m_CommandCount = 0;
				m_CommandCount = 0;
				if (m_CommandTimer.isActive()) {
					m_CommandTimer.stop();
				}
			}
		}
	}
}
