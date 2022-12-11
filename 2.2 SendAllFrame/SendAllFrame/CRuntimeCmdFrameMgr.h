#pragma once
#include "ICmdFrameMgr.h"
class CRuntimeCmdFrameMgr :
    public ICmdFrameMgr
{
public:
    explicit CRuntimeCmdFrameMgr(std::shared_ptr<CUDPService> _udpservice);

    /// @brief ����ͨ�ſ�������
    /// @param _id �豸id
    /// @param _isforbiddata �豸ͨ��Լ����0-���������ݣ�1-��ֹ��������
    void ExecCommand(int _id);

protected:
    virtual void _ParseFrame(UInt8* const _recvframe) override;

private:
    /// @brief ������ʱ����Сʱ����Ԫ��0-5�ֱ�Ϊ1-6·������Ԫ��6-9Ϊ1-4·ֱ��
    Int32 m_OpenTime[10] = {0};
};

