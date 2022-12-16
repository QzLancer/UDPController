#pragma once
#include "IFrameMgr.h"

/// @brief ��Ϣ֡��PHM������,����������������Ϣ
class CMsgFrameMgr :
    public IFrameMgr
{
public:
    explicit CMsgFrameMgr(std::shared_ptr<CUDPService> _udpservice);

    virtual void _ParseFrame(UInt8* const _recvframe) override;

private:
    /// @brief ���״̬��0x01��������0x02��Ԥ����0x03��������0x04������
    UInt8 m_CombinationStatus = 0;

    /// @brief �������ģ��״̬��0x01�������� 0x04�����ϣ�
    UInt8 m_ACPowerStatus = 0;

    /// @brief ֱ�����ģ��״̬��0x01�������� 0x04�����ϣ�
    UInt8 m_DCPowerStatus = 0;

    /// @brief ���״̬��0x01�������� 0x04�����ϣ�
    UInt8 m_FanStatus = 0;

    /// @brief �̵�������������Ԫ��0-Ԫ��5�ֱ�Ϊ1-6·������Ԫ��6-Ԫ��9Ϊ1-4·ֱ��
    UInt16 m_RelayActCnt[10] = { 0 };

    /// @brief ��ǰ�豸�¶�ֵ
    Int8 m_DeviceTemp;

    /// @brief ���������ѹ
    UInt16 m_InputACVoltage;

    /// @brief �����ѹ��Ԫ��0-Ԫ��5�ֱ�Ϊ1-6·������Ԫ��6-Ԫ��9Ϊ1-4·ֱ��
    UInt16 m_OutputVoltage[10] = { 0 };

    /// @brief ���������Ԫ��0-Ԫ��5�ֱ�Ϊ1-6·������Ԫ��6-Ԫ��9Ϊ1-4·ֱ��
    UInt16 m_OutputVoltageCurrent[10] = { 0 };
};

