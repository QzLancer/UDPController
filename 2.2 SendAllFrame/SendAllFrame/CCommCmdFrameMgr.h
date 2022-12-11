#pragma once
#include "ICmdFrameMgr.h"
class CCommCmdFrameMgr :
    public ICmdFrameMgr
{
public:
    explicit CCommCmdFrameMgr(std::shared_ptr<CUDPService> _udpservice);

    /// @brief ����ͨ�ſ�������
    /// @param _id �豸id
    /// @param _isforbiddata �豸ͨ��Լ����0-���������ݣ�1-��ֹ��������
    void ExecCommand(int _id, bool _isforbidsend);

protected:
    virtual void _ParseFrame(UInt8* const _recvframe) override;

private:

    /// @brief ͨ��״̬
    bool m_CommState = false;

    /// @brief 1~8�Žڵ�CANͨѶ��־λ��0������1���ϣ�
    bool m_CAN_Error[8] = {0};
};

