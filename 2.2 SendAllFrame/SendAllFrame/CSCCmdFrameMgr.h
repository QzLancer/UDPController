#pragma once

#include "ICmdFrameMgr.h"

/// @brief �豸�Լ�֡����
class CSCCmdFrameMgr : public ICmdFrameMgr
{
	Q_OBJECT
public:
	explicit CSCCmdFrameMgr(std::shared_ptr<CUDPService> _udpservice);

	void ExecCommand(int _id);

	virtual void _ParseFrame(UInt8* const _recvframe) override;

private:

	/// @brief �ӵ����״̬��Bit0:����1���ػ�0��Bit1������0��ң��1��Bit2����Դ��ֱ������ѹ1������0��Bit3��������ѹ1������0��Bit4������Ƿѹ1������0
	bool m_PowerStatus[5] = { false };

	/// @brief ��ѹ�쳣��Bit0-Bit5�ֱ�Ϊ1-6·������Bit6-Bit9Ϊ1-4·ֱ��
	bool m_OverVoltage[10] = { false };

	/// @brief Ƿѹ�쳣��Bit0-Bit5�ֱ�Ϊ1-6·������Bit6-Bit9Ϊ1-4·ֱ��
	bool m_UnderVoltage[10] = { false };

	/// @brief �����쳣��Bit0-Bit5�ֱ�Ϊ1-6·������Bit6-Bit9Ϊ1-4·ֱ��
	bool m_OverCurrent[10] = { false };

	/// @brief �����쳣��Bit0-Bit5�ֱ�Ϊ1-6·������Bit6-Bit9Ϊ1-4·ֱ��
	bool m_OverTemp[10] = { false };

	/// @brief ������ʱ���Է���Ϊ��λ
	UInt32 m_TotalStartupTime = 0;
};

