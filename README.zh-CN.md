# 小猫环境温湿度监测器

[English](README.md) | **简体中文**

一个基于 ESP32 的环境监测原型，用于查看小猫休息区域附近的空气温度和湿度。两行液晶屏显示读数，温度超出设定区间时，LED 和蜂鸣器发出提示。

[查看 Arduino 代码](firmware/KittenMonitor/KittenMonitor.ino) · [项目图片目录](assets/)

<img src="assets/prototype-overview.jpg" alt="整体原型：黄色箱盖上的 ESP32、面包板接线、液晶屏和外部供电设备" width="560">

*整体原型及其监测电路。*

## 已实现功能

- 使用 DHT11 读取温度和湿度。
- 在 I²C LCD1602 上显示温度、湿度和 `COLD`、`HOT`、`OK` 状态。
- 低温使用 800 Hz、较慢节奏的提示音；高温使用 1500 Hz、较快节奏的提示音。
- 通过 115200 波特率的串口输出读数和状态。
- 读取失败时显示 `DHT11 ERROR` 和 `Check wiring`。

屏幕实际是 **LCD1602＋PCF8574 I²C 转接板**，不是 OLED。湿度仅用于显示，不触发报警。

## 屏幕显示与传感器位置

<img src="assets/lcd-readout.jpg" alt="LCD1602 特写，显示 Temp 26.7 C 和 Hum 60% OK" width="500">

*屏幕显示空气温度、相对湿度及当前状态。*

<img src="assets/sensor-placement.jpg" alt="悬挂在休息箱内、位于幼猫垫毯上方的 DHT11 传感器" width="400">

*DHT11 放置在休息箱内部、垫毯上方。*

## 硬件与信号引脚

| 元件 | ESP32 信号引脚 |
| --- | --- |
| DHT11 数据引脚 | GPIO4 |
| LED，经限流电阻连接 | GPIO18 |
| 蜂鸣器 | GPIO19 |
| LCD SDA | GPIO21 |
| LCD SCL | GPIO22 |

各元件需要共地。供电和 I²C 电压兼容性应按实际模块核对；以上表格记录的是现有代码的信号引脚分配。

## 打开与运行

1. 在 Arduino IDE 安装 **Espressif Systems** 的 **esp32** 开发板包。
2. 在库管理器安装 **Adafruit** 的 **DHT sensor library**、其依赖 **Adafruit Unified Sensor**，以及 **Bill Perry** 的 **hd44780**。
3. 打开 [`firmware/KittenMonitor/KittenMonitor.ino`](firmware/KittenMonitor/KittenMonitor.ino)。
4. 选择实际使用的 ESP32 开发板和端口，上传后以 **115200** 波特率打开串口监视器。

代码使用 `hd44780_I2Cexp` 自动识别屏幕的 I²C 地址和转接板配置。原草稿没有记录依赖库的准确版本。

## 保留设置与当前范围

现有代码设置 `MIN_TEMP = 27.0`、`MAX_TEMP = 31.0`：低于 27°C 或高于 31°C 报警，恰好等于上下限时仍显示 `OK`。这是原型中保留的历史设置，不是适用于所有小猫的推荐温度范围。

屏幕照片显示 `26.7°C`、`60%` 和 `OK`，而保留的代码在低于 27°C 时应显示 `COLD`，说明照片中的版本与保存代码采用了不同的阈值设置。本次没有为匹配照片而修改原代码。

程序测量的是**空气温度**，不是小猫体温，也没有实现加热器或风扇控制。DHT 读取失败时，屏幕显示错误，同时关闭 LED 和蜂鸣器。程序使用阻塞式延时，没有数据记录、联网或独立保护控制。

## 开发记录

我在 ChatGPT 帮助生成代码、解释接线和程序行为的过程中完成了这个原型。项目始于 2026 年 9 月。本仓库保留的是从本机 Arduino 编译缓存中找回的版本，缓存日期为 2026 年 9 月 7 日；程序逻辑及当时的温度设置均保持原样。

本次发布是对原代码的整理保存，没有重新编译或连接硬件测试。

## 幼猫休息区

<img src="assets/kittens-resting.jpg" alt="两只幼猫在箱内的垫毯上依偎休息" width="560">

*幼猫在休息区内依偎；原型用于监测该区域周围的空气。*

这些照片展示原型实物及其使用场景。完整图片保存在 [`assets/`](assets/) 目录。
