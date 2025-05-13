<!--
 * @Author: qwqb233 qwqb.zhang@gmail.com
 * @Date: 2025-05-13 21:43:34
 * @LastEditors: qwqb233 qwqb.zhang@gmail.com
 * @FilePath: \Scratch-head-1\stm32\按键模块\README.md
 * @Description: 
-->

# 按键模块使用说明

## 功能简介
该按键模块提供了基础的按键检测功能，支持以下特性：
- 支持多个按键同时使用
- 支持按键按下、释放和长按检测
- 支持按键状态先进先出队列
- 跨平台兼容设计

## 数据结构
### 按键状态枚举 (button_state_t)
```c
typedef enum {
    button_pressed,   // 按键按下
    button_released,  // 按键释放
    button_hold,      // 按键长按
    button_none       // 无动作
} button_state_t;
```

### 按键结构体 (button_t)
```c
typedef struct button {
    GPIO_TypeDef *port;  // 按键端口
    uint16_t pin;        // 按键引脚
    button_state_t state;// 按键状态
    uint32_t hold_time;  // 长按检测时间(ms)
} button_t;
```

## 移植指南

### 1. 添加新按键
在 `button_init()` 函数中添加按键配置：
```c
void button_init(void)
{
    // 示例：配置一个按键
    button_t button;
    button_struct_init(&button);    // 初始化结构体
    button.pin = BUTTON_PIN;        // 设置引脚
    button.port = BUTTON_PORT;      // 设置端口
    button.hold_time = 1000;        // 设置长按检测时间(ms)，设为0则关闭长按检测
}
```

### 2. 移植到其他单片机
1. 修改按键结构体中的端口和引脚类型：
```c
typedef struct button {
    // 根据目标平台修改类型
    PORT_Type *port;   // 改为对应单片机的端口类型
    pin_Type pin;      // 改为对应单片机的引脚类型
    button_state_t state;
    uint32_t hold_time;
} button_t;
```

2. 修改GPIO相关操作函数以匹配目标平台的API。

## 使用注意事项
1. 使用前需要先调用 `button_init()` 进行初始化
2. 需要定期调用 `button_scan()` 进行按键扫描
3. 按键数量在 `BUTTON_NUM` 宏中定义
4. 确保正确配置目标平台的GPIO时钟和模式

## 示例代码
```c
// 初始化
button_init();

// 主循环中,可以在定时中断中调用button_scan()进行按键扫描
while(1) {
    button_scan();
    // 其他代码
}
```
