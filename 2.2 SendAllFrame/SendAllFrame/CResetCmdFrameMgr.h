#pragma once
#include "ICmdFrameMgr.h"

/// @brief 复位帧管理
class CResetCmdFrameMgr :
    public ICmdFrameMgr
{
public:
    explicit CResetCmdFrameMgr(std::shared_ptr<CUDPService> _udpservice);

    /// @brief 发送上电控制命令
    /// @param _id 设备id
    /// @param _deviceselece 设备选择，0-无效，1-有效，Bit0-Bit5分别为1-6路交流，Bit6-Bit9为1-4路直流
    /// @param _isonpower 
    void ExecCommand(int _id, bool _deviceselect[10], bool _isonpower);

    virtual void _ParseFrame(UInt8* const _recvframe) override;

private:
    /// @brief 上电状态，Bit0-Bit5分别为1-6路交流，Bit6-Bit9为1-4路直流
    bool m_bIsSuccess = false;
};

