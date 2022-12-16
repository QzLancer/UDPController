#pragma once
#include "ICmdFrameMgr.h"

/// @brief ��λ֡����
class CResetCmdFrameMgr :
    public ICmdFrameMgr
{
public:
    explicit CResetCmdFrameMgr(std::shared_ptr<CUDPService> _udpservice);

    /// @brief �����ϵ��������
    /// @param _id �豸id
    /// @param _deviceselece �豸ѡ��0-��Ч��1-��Ч��Bit0-Bit5�ֱ�Ϊ1-6·������Bit6-Bit9Ϊ1-4·ֱ��
    /// @param _isonpower 
    void ExecCommand(int _id, bool _deviceselect[10], bool _isonpower);

    virtual void _ParseFrame(UInt8* const _recvframe) override;

private:
    /// @brief �ϵ�״̬��Bit0-Bit5�ֱ�Ϊ1-6·������Bit6-Bit9Ϊ1-4·ֱ��
    bool m_bIsSuccess = false;
};

