#pragma once
#include <stdint.h>

// 特定数据类型的定义
#define Int8 int8_t
#define UInt8 uint8_t  
#define Int16 int16_t
#define UInt16 uint16_t
#define Int32 int32_t
#define UInt32 uint32_t
#define Int64 int64_t

/// @brief 定义协议数据单元（PDU）结构体
struct PDUStruct {
    UInt8 Head1 = 0xA5;    //帧头1
    UInt8 Head2 = 0x5A;    //帧头2
    UInt8 N = 0;    //帧长度
    UInt8 SA = 0;    //源ID
    UInt8 DA = 0;    //目标ID
    UInt8 PF1 = 0;  //功能码1
    UInt8 PF2 = 0;  //功能码2
    UInt8 FrameData[255] = { '\0' }; //帧数据
};