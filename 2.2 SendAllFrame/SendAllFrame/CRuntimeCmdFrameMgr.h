#pragma once
#include "ICmdFrameMgr.h"
class CRuntimeCmdFrameMgr :
    public ICmdFrameMgr
{
public:
    explicit CRuntimeCmdFrameMgr(std::shared_ptr<CUDPService> _udpservice);

    /// @brief 发送通信控制命令
    /// @param _id 设备id
    /// @param _isforbiddata 设备通信约束，0-允许发送数据，1-禁止发送数据
    void ExecCommand(int _id);

protected:
    virtual void _ParseFrame(UInt8* const _recvframe) override;

private:
    /// @brief 开机总时长（小时），元素0-5分别为1-6路交流，元素6-9为1-4路直流
    Int32 m_OpenTime[10] = {0};
};

