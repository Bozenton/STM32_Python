#ifndef USART2_H_
#define USART2_H_

#include "stm32f10x.h"
#include "sys.h"

// 缓冲区大小
#define MY_RXBUFF_SIZE    64
#define MY_TXBUFF_SIZE    8

#define MY_PIN_TX               GPIO_Pin_2              // TX: 
#define MY_PIN_RX               GPIO_Pin_3              // RX: 
#define MY_GPIO                 GPIOA
#define MY_IRQn                 USART2_IRQn
#define MY_USART                USART2
#define MY_RCC_USART            RCC_APB1Periph_USART2
#define MY_RCC_GPIO             RCC_APB2Periph_GPIOA
#define MY_PreemptionPriority   5                       // 抢占优先级
#define MY_SubPriority          0                       // 子优先级
#define MY_BaudRate             115200                   // 波特率

/* ------------------------------------- 数据包格式 -------------------------------------
字节数      数据        说明
1           0xFF       包头
1           0x         字节长度（数据部分） 0~254
1           0x         该字节用于表示数据类型
n           ...        data部分
1           0x         校验和，对数据部分累加取低八位

*/
// 通用
#define MY_PACK_HEAD  0xff
#define MY_RXPACK_UNFINISHED      0   // 开始了但是没结束
#define MY_RXPACK_FINISHED        1   // 成功完成
#define MY_RXPACK_CHECK_FAILED    2   // 出现错误
#define MY_RXPACK_UNKNOWN_FAILED  3   // 未知错误
#define MY_RXSTA_HEAD_BIT         0x8000 // bit15 标记收到包头
#define MY_RXSTA_LEN_BIT          0x4000 // bit14 标记收报数据长度位
#define MY_RXSTA_TYPE_BITS        0x3800 // bit13,12,11 标记接收到的数据类型
#define MY_RXSTA_DATA_BIT         0x400  // bit10 标记数据部分接收完成
#define MY_RXSTA_CNT_BITS         0xff   // bit7~0 对数据部分的计数

void initUsart2(void);



#endif


