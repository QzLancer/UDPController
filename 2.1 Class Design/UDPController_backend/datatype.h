#pragma once
#include <stdint.h>

// �ض��������͵Ķ���
#define Int8 int8_t
#define UInt8 uint8_t  
#define Int16 int16_t
#define UInt16 uint16_t
#define Int32 int32_t
#define UInt32 uint32_t
#define Int64 int64_t

/// @brief ����Э�����ݵ�Ԫ��PDU���ṹ��
struct PDUStruct {
    UInt8 Head1 = 0xA5;    //֡ͷ1
    UInt8 Head2 = 0x5A;    //֡ͷ2
    UInt8 N = 0;    //֡����
    UInt8 SA = 0;    //ԴID
    UInt8 DA = 0;    //Ŀ��ID
    UInt8 PF1 = 0;  //������1
    UInt8 PF2 = 0;  //������2
    UInt8 FrameData[255] = { '\0' }; //֡����
};