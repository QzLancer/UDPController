#pragma once
#include "IFrameMgr.h"

/// @brief 信息帧（PHM）管理,被动侦听控制器信息
class CMsgFrameMgr :
    public IFrameMgr
{
public:
    explicit CMsgFrameMgr(std::shared_ptr<CUDPService> _udpservice);

    virtual void _ParseFrame(UInt8* const _recvframe) override;

private:
    /// @brief 组合状态，0x01：正常；0x02：预警；0x03：降级；0x04：故障
    UInt8 m_CombinationStatus = 0;

    /// @brief 交流配电模块状态，0x01：正常； 0x04：故障；
    UInt8 m_ACPowerStatus = 0;

    /// @brief 直流配电模块状态，0x01：正常； 0x04：故障；
    UInt8 m_DCPowerStatus = 0;

    /// @brief 风机状态，0x01：正常； 0x04：故障；
    UInt8 m_FanStatus = 0;

    /// @brief 继电器动作次数，元素0-元素5分别为1-6路交流，元素6-元素9为1-4路直流
    UInt16 m_RelayActCnt[10] = { 0 };

    /// @brief 当前设备温度值
    Int8 m_DeviceTemp;

    /// @brief 交流输入电压
    UInt16 m_InputACVoltage;

    /// @brief 输出电压，元素0-元素5分别为1-6路交流，元素6-元素9为1-4路直流
    UInt16 m_OutputVoltage[10] = { 0 };

    /// @brief 输出电流，元素0-元素5分别为1-6路交流，元素6-元素9为1-4路直流
    UInt16 m_OutputVoltageCurrent[10] = { 0 };
};

