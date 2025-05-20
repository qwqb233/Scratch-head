<!--
 * @Author: qwqb233 qwqb.zhang@gmail.com
 * @Date: 2025-05-20 12:28:18
 * @LastEditors: qwqb233 qwqb.zhang@gmail.com
 * @FilePath: \Scratch-head-1\stm32\按键模块\README.md
 * @Description: 
-->

# STM32按键模块

## 功能特点
- 支持多按键同时检测
- 支持按下、释放和长按检测
- 带有按键消抖功能
- 使用FIFO缓冲按键事件
- 支持自定义长按时间
- 支持跨平台移植

## 使用方法

### 1. 基本配置
```c
// 配置按键数量和FIFO大小
#define BUTTON_FIFO_SIZE 10  // FIFO缓冲区大小

```

### 2. 按键事件类型
```c
typedef enum
{
    button_none = 0,        // 无按键事件
    start_button_pressed,   // START按键按下
    start_button_long_pressed, // START按键长按
    start_button_released,  // START按键释放
    select_button_pressed,  // SELECT按键按下
    select_button_long_pressed, // SELECT按键长按
    select_button_released, // SELECT按键释放
    button_max,
} button_t;
#define BUTTON_COUNT     (uint8_t)(button_max-1)/3   // 实际按键数量
```


### 3. 初始化配置
在button_init()函数中配置按键：
```c
void button_init(void)
{
    // 配置START按键
    button_list[0].port = Start_button_GPIO_Port;
    button_list[0].pin = Start_button_Pin;
    button_list[0].hold_time = 10;  // 长按检测时间

    // 配置SELECT按键
    button_list[1].port = Select_button_GPIO_Port;
    button_list[1].pin = Select_button_Pin;
    button_list[1].hold_time = 10;
}
```

## 移植指南

### 1. 修改GPIO类型定义
根据目标平台修改以下定义：
```c
#define GPIO_PORT_TYPE GPIO_TypeDef*   // 修改为目标平台的GPIO端口类型
#define GPIO_PIN_TYPE uint16_t         // 修改为目标平台的GPIO引脚类型
```

### 2. 实现硬件初始化（可选）
如需要硬件初始化，开启BUTTON_HARD_INIT并实现初始化函数：
```c
#define BUTTON_HARD_INIT  // 取消注释启用硬件初始化

static inline void button_hard_init(button_struct_t *button)
{
    // 实现目标平台的GPIO初始化
}
```

### 3. 按键扫描
提供两种扫描方式：
```c
void button_scan_1ms(void);   // 1ms定时器中调用
void button_scan_10ms(void);  // 10ms定时器中调用
```

## 使用示例

```c
int main(void)
{
    // 初始化按键
    button_init();
    
    while(1)
    {
        // 读取按键事件
        uint8_t key_event = button_fifo_read();
        switch(key_event)
        {
            case start_button_pressed:
                // 处理START按键按下事件
                break;
            case start_button_long_pressed:
                // 处理START按键长按事件
                break;
            // ... 处理其他按键事件
        }
        
        // 1ms扫描方式
        button_scan_1ms();
        HAL_Delay(1);
    }
}
```

## 注意事项
1. 按键扫描函数需要定期调用，可放在定时器中断中
2. 确保FIFO缓冲区大小足够存储按键事件
3. 移植时注意修改GPIO相关操作为目标平台对应接口
4. 按键消抖时间可通过filter_time参数调整
5. 长按检测时间可通过hold_time参数调整，设为0可禁用长按检测
