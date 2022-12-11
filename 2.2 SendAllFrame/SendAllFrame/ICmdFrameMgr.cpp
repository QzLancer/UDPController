#include "ICmdFrameMgr.h"

ICmdFrameMgr::ICmdFrameMgr(std::shared_ptr<CUDPService> _udpservice):
	IFrameMgr(_udpservice)
{
	connect(&m_CommandTimer, SIGNAL(timeout()), this, SLOT(__slotSendPDU()));
}

ICmdFrameMgr::~ICmdFrameMgr()
{
	if (m_SendFrame != NULL) {
		delete[] m_SendFrame;
	}
	if (m_RecvFrame != NULL) {
		delete[] m_RecvFrame;
	}
}

void ICmdFrameMgr::__slotSendPDU()
{
	m_UDPService->ConvertPDUtoFrame(m_PDUStruct, m_SendFrame);

	if (m_UDPService->SendFrame(m_SendFrame, m_SendFrameSize, 1)) {
		if (m_UDPService->RecvFrame(m_RecvFrame, m_RecvFrameSize, 1)) {
			printf("Message: Received self check response.\n");
			printf("frame: ");
			for (int i = 0; i < m_RecvFrameSize; ++i) {
				printf("%#X ", m_RecvFrame[i]);
			}
			printf("\n");
			_ParseFrame(m_RecvFrame);
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
				if (m_CommandTimer.isActive()) {
					m_CommandTimer.stop();
				}
			}
		}
	}
}
