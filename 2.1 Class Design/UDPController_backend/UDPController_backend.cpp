// UDPController_backend.cpp : UDP控制器的后端测试
// 2022/12/6
// By QzLancer

#include "CUDPService.h"

int main()
{
	//CUDPServiceTest
	CUDPService UDPtest;
	UDPtest.SetHostIP("192.168.199.235");
	UDPtest.SetInitHostPort(8004);
	UDPtest.SetControllerIP("192.168.199.235");
	UDPtest.SetInitControllerPort(8005);
	UDPtest.SetControllerID(1);

	UDPtest.BindSocket();
	UInt8 send[5] = { 'N','E','I','O','U'};
	while (1) {
		UDPtest.SendFrame(send, 5);
	}

}