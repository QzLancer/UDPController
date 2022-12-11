#include "CMsgFrameMgr.h"

CMsgFrameMgr::CMsgFrameMgr(std::shared_ptr<CUDPService> _udpservice) : 
	IFrameMgr(_udpservice)
{
	m_RecvFrameSize = 79;
	m_RecvFrame = new UInt8[m_RecvFrameSize];
}

void CMsgFrameMgr::_ParseFrame(UInt8* const _recvframe)
{

}
