#include "IFrameMgr.h"

IFrameMgr::IFrameMgr(std::shared_ptr<CUDPService> _udpservice):
	m_UDPService(_udpservice)

{
	m_PDUStruct.Head1 = 0xA5;
	m_PDUStruct.Head2 = 0x5A;
	m_PDUStruct.SA = 0x01;
}

IFrameMgr::~IFrameMgr()
{
	if (m_SendFrame != NULL) {
		delete[] m_SendFrame;
	}
	if (m_RecvFrame != NULL) {
		delete[] m_RecvFrame;
	}
}
