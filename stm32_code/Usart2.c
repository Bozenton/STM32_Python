#include "Usart2.h"

uint8_t my_rxbuff[MY_RXBUFF_SIZE];

void initUsart2(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(MY_RCC_USART, ENABLE);
    RCC_APB2PeriphClockCmd(MY_RCC_GPIO, ENABLE);

    USART_DeInit(MY_USART);

    // USART2_TX PA2
    GPIO_InitStructure.GPIO_Pin = MY_PIN_TX;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(MY_GPIO, &GPIO_InitStructure);

    // USART2_RX PA3
    GPIO_InitStructure.GPIO_Pin = MY_PIN_RX;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
    GPIO_Init(MY_GPIO, &GPIO_InitStructure);

    // USART2 NVIC
    NVIC_InitStructure.NVIC_IRQChannel = MY_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = MY_PreemptionPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = MY_SubPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

    // USART4
    USART_InitStructure.USART_BaudRate = MY_BaudRate;           // 波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; // 字长8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;      // 一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;         // 无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // 收发模式
    USART_Init(MY_USART, &USART_InitStructure);
    USART_ClearITPendingBit(MY_USART, USART_IT_RXNE);
    USART_ITConfig(MY_USART, USART_IT_RXNE, ENABLE);
    USART_Cmd(MY_USART, ENABLE);
}

uint16_t MY_REC_STA = 0; // 接收状态
// bit15 标记收到包头
// bit14 标记收报数据长度位
// bit13,12,11 标记接收到的数据类型
// bit10 标记数据部分接收完成
// bit7~0 对数据部分的计数
uint8_t MY_RXPACK_LEN = 0; // 存放接收数据包的
uint8_t MY_RXPACK_CHECK = 0; // 校验和


/**
 * @brief openmv所用串口的数据解析函数
 * 
 * @param recbyte 接收到的字节
 * @return uint8_t 解析状态
 *      MY_RXPACK_UNFINISHED      0   // 开始了但是没结束
 *      MY_RXPACK_FINISHED        1   // 成功完成
 *      MY_RXPACK_CHECK_FAILED    2   // 出现错误
 *      MY_RXPACK_UNKNOWN_FAILED  3   // 未知错误
 */

uint8_t prvParseChar(uint8_t recbyte)
{
    if((recbyte == MY_PACK_HEAD) && ((MY_REC_STA&MY_RXSTA_HEAD_BIT)==0)) // 收到包头且head位为0
    {
        MY_REC_STA = 0;
        MY_REC_STA |= MY_RXSTA_HEAD_BIT; // 标记已经收到包头
        MY_RXPACK_CHECK = 0;
        return MY_RXPACK_UNFINISHED;
    }
    else if( MY_REC_STA & MY_RXSTA_HEAD_BIT ) // 已经收到包头
    {
        if( (MY_REC_STA & MY_RXSTA_LEN_BIT) == 0) // 收到长度位
        {
            MY_REC_STA |= MY_RXSTA_LEN_BIT;
            MY_RXPACK_LEN = recbyte; // data部分的字节长度
            
            // printf("len=%d\r\n", recbyte);
        }
        else if( (MY_REC_STA&MY_RXSTA_TYPE_BITS) == 0 ) // 收到类型位
        {
            MY_REC_STA |= ((uint16_t)recbyte) << 11;
        }
        else if( ((MY_REC_STA & MY_RXSTA_DATA_BIT)==0)&&(MY_RXPACK_LEN>0) ) // 收到数据部分
        {
            uint8_t idx = (uint8_t)(MY_REC_STA&MY_RXSTA_CNT_BITS);

            my_rxbuff[idx] = recbyte;
            MY_RXPACK_CHECK += recbyte;
            MY_REC_STA ++;

            if( (idx+1) >= MY_RXPACK_LEN ) // 数据部分已经接收完最后一个字节
            {
                MY_REC_STA |= MY_RXSTA_DATA_BIT;
            }
        }
        else // 收到校验位
        {
            if( recbyte != MY_RXPACK_CHECK )
                return MY_RXPACK_CHECK_FAILED;
            return MY_RXPACK_FINISHED;
        }
        return MY_RXPACK_UNFINISHED;
    }
    else // 未知错误
        return MY_RXPACK_UNKNOWN_FAILED;
}

/**
 * @brief 串口接收中断服务函数
 * 
 */
void USART2_IRQHandler(void)
{
    if(USART_GetITStatus(MY_USART, USART_IT_RXNE))
    {
        uint8_t recbyte = USART_ReceiveData(MY_USART);
        uint8_t flag = prvParseChar(recbyte);
        // printf("f:%d\r\n", flag);

        switch(flag)
        {
            case MY_RXPACK_UNFINISHED: // 正在进行中
            {
                break;
            }
            case MY_RXPACK_FINISHED: // 接收完成
            {

                /***************************************************************************************/
                
                // WRITE YOUR CODE HERE

                /***************************************************************************************/
                
                MY_REC_STA = 0;
                break;
            }
            case MY_RXPACK_CHECK_FAILED: // 出现错误
            {
                /***************************************************************************************/
                
                // WRITE YOUR CODE DEALING WITH CHECK ERROR HERE

                /***************************************************************************************/
                
                MY_REC_STA = 0;
                break;
            }
            case MY_RXPACK_UNKNOWN_FAILED: // 出现错误
            {
                /***************************************************************************************/
                
                // WRITE YOUR CODE DEALING WITH ERROR HERE

                /***************************************************************************************/
                
                MY_REC_STA = 0;
                break;
            }
        }
    }// end if
}


