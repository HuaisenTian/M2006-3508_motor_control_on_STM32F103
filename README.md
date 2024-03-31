# M2006-3508_motor_control_on_STM32F103
通过STM32F103系列核心板实现对大疆M2006-3508电机的PID控制，官网给出的寄存器版主要是基于F407系列的，没有移植到F103系列。

代码主要实现：CAN封装，速度控制环，位置控制环，串口设定目标值



## 展示包含硬件

1. STM32F103核心板
2. CAN收发器
3. mini电源
4. 串口
5. 2006电机
6. C610电调

![stm32f103.jpg](https://github.com/tianhuaisen/M2006-3508_motor_control_on_STM32F103/blob/main/pic_video/stm32f103.jpg)



## 实机演示

![2006PID.gif](https://github.com/tianhuaisen/M2006-3508_motor_control_on_STM32F103/blob/main/pic_video/2006PID.gif)

