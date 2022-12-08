// UDPController_backend.cpp : UDP控制器的后端测试
// 2022/12/6
// By QzLancer

#include "CUDPService.h"
#include "CSyncFrame.h"
#include "CCmdFrame.h";

int main()
{
	CUDPService* udpservice = new CUDPService;

	CCmdFrame cmdframe(udpservice);

	if (udpservice == NULL) delete udpservice;
}