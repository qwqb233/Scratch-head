<!--
 * @Author: qwqb233 qwqb.zhang@gmail.com
 * @Date: 2025-05-13 21:15:25
 * @LastEditors: qwqb233 qwqb.zhang@gmail.com
 * @FilePath: \Scratch-head\README.md
 * @Description: 
-->

---

## 🧩 主要内容说明

#### _人们要想得到工作的胜利即得到预想的结果，一定要使自己的思想合于客观世界的规律性，_
#### _如果不合，就会在实践中失败。人们经过失败之后，也就从失败取得教训，改正自己的思想_
#### _使之适合于外界的规律性，人们就能变失败为胜利_
##### _——毛泽东_

### ~~🎯 K210 机器视觉部分（已搁置）~~
- ~~包含图像采集、目标识别、神经网络模型部署等内容~~
- ~~支持使用 MaixPy 或 C SDK 进行开发~~

### 🎯树莓派机器视觉部分
- 包含C++ class qt6、opencv、云台控制
- QT包含边缘检测参数调节、PID参数调节
_叽里咕噜说些什么呢，我就是要用软实时系统做控制_

### 📱 MSPM 德州仪器单片机部分
- 基于 MSPM0 系列芯片的开发资料
- 使用Torris-Yin开发的部分驱动https://github.com/Torris-Yin/mspm0-modules
- 惯性导航轨迹跟踪避免尖峰脉冲，灰度巡线校准避免零漂

### ~~🔧 STM32 意法半导体单片机部分（已搁置）~~
- ~~使用 STM32CubeMX + HAL 库的标准工程模板~~
- ~~包含串口通信、SPI、I2C、FreeRTOS 等常用功能示例~~

---

## 📚 使用建议

```shell
# http
git clone https://github.com/Torris-Yin/mspm0-modules.git
# ssh
git clone git@github.com:Torris-Yin/mspm0-modules.git

# enter the directory
git submodule update --init --recursive
```


---