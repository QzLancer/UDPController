#pragma once

#include "ICmdFrameMgr.h"

/// @brief 设备自检帧管理
class CSCCmdFrameMgr : public ICmdFrameMgr
{
	Q_OBJECT
public:
	explicit CSCCmdFrameMgr(std::shared_ptr<CUDPService> _udpservice);

	void ExecCommand(int _id);

	virtual void _ParseFrame(UInt8* const _recvframe) override;

private:

	/// @brief 加电组合状态，Bit0:开机1，关机0；Bit1：本控0，遥控1；Bit2：电源（直流）过压1，正常0；Bit3：交流过压1，正常0；Bit4：交流欠压1，正常0
	bool m_PowerStatus[5] = { false };

	/// @brief 过压异常，Bit0-Bit5分别为1-6路交流，Bit6-Bit9为1-4路直流
	bool m_OverVoltage[10] = { false };

	/// @brief 欠压异常，Bit0-Bit5分别为1-6路交流，Bit6-Bit9为1-4路直流
	bool m_UnderVoltage[10] = { false };

	/// @brief 过流异常，Bit0-Bit5分别为1-6路交流，Bit6-Bit9为1-4路直流
	bool m_OverCurrent[10] = { false };

	/// @brief 过温异常，Bit0-Bit5分别为1-6路交流，Bit6-Bit9为1-4路直流
	bool m_OverTemp[10] = { false };

	/// @brief 开机总时长以分钟为单位
	UInt32 m_TotalStartupTime = 0;
};

