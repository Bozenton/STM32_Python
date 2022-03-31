# STM32 communicate with Python

## Environment

* Python 3.6
* stm32f103c6t6

## Hardware

Connect the USB port of PC with usart2 of stm32f103c6t6. *USB to TTL converter* may be needed. 

## Simple protocol 

Structure of the message package:

| HEAD   | LENTH  | TYPE   | DATA   | CHECK  |
| ------ | ------ | ------ | ------ | ------ |
| 1 byte | 1 byte | 1 byte | n byte | 1 byte |

* HEAD: the head of the package, here is `0xff`. 
* LENTH: the number of bytes in the part of DATA.  
* TYPE: the type of data. Number `1` to `7` can be used. **Note that `0` is unavailable**.  
* DATA: data in the message. 
* CHECK: low 8 bit of the sum of the data. 

## STM32

Use Usart to receive message from computer. 

Use a `uint16_t` variable `MY_REC_STA` to record the state of receiving data. 

* bit15: receiving flag of head of the package
* bit14: receiving flag of length
* bit13,12,11: type of the data
* bit10: flag of finishing receiving data 
* bit7~0: counter when receiving data 

### How to use it

Put the `initUsart2()` in your main function. Remember to set priority group by function`NVIC_PriorityGroupConfig()`. 

Write your code or callback function in `Usart2_IRQHandler()` (in the file `Usart2.c`) to address the received data. 

Some parameters, such as baud, buffer size, etc, can be modified in the file `Usart2.h`.  


## Python

Class `SendMsg` is defined to pack the message and send it to stm32 through serial. 

## How to use it

Use the method `send(self, msg_type : bytes, data : list)` 

Remember to use the same baud with stm32

