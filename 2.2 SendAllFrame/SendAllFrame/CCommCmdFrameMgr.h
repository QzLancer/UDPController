#pragma once
#include "ICmdFrameMgr.h"
class CCommCmdFrameMgr :
    public ICmdFrameMgr
{
public:
    explicit CCommCmdFrameMgr(std::shared_ptr<CUDPService> _udpservice);

    /// @brief 发送通信控制命令
    /// @param _id 设备id
    /// @param _isforbiddata 设备通信约束，0-允许发送数据，1-禁止发送数据
    void ExecCommand(int _id, bool _isforbidsend);

protected:
    virtual void _ParseFrame(UInt8* const _recvframe) override;

private:

    /// @brief 通信状态
    bool m_CommState = false;

    /// @brief 1~8号节点CAN通讯标志位（0正常，1故障）
    bool m_CAN_Error[8] = {0};
};

