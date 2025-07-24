"""
数字识别 （分类）
波特率：115200
数据包：
实验例程        开始符	  例程编号	数据量	数字ID	分隔符	结束符	备注
手写数字识别      0xfe	11       01      XXX	  ,      0xff    ID:是指识别出来的数字ID（0-9）

目前参数适合制药小车
"""

import sensor, image, time, lcd
from maix import KPU
import gc
from modules import ybserial
import time
import binascii

# 初始化串口并配置参数
uart = ybserial(
    uart_num=1,          # UART 端口号 (1, 2, 3)
    baudrate=115200,     # 波特率
    bits=8,              # 数据位 (5, 6, 7, 8)
    parity='N',          # 校验位: 'N' (无), 'E' (偶校验), 'O' (奇校验)
    stop=1,              # 停止位 (1, 1.5, 2)
    timeout=1000         # 超时时间 (毫秒)
)


def send_data(digit):
    start = 0xfe
    class_num = 0x0B   # 例程编号
    data_num = 0x01    # 数据量固定为1
    fenge = 0x2c       # 分隔符
    end = 0xff

    # 构造数据包：[开始符, 例程编号, 数据量, 数字值, 分隔符, 结束符]
    send_merr = [start, class_num, data_num, digit, fenge, end]

    global send_buf
    send_buf = send_merr
    print("Send buf:", send_buf)


send_buf = []
msg_ = ""


count_dict = {i: 0 for i in range(10)}  # 数字0-9的计数器
last_send_time = time.ticks_ms()        # 上次发送时间
interval = 2000                         # 2秒统计间隔

lcd.init()
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_windowing((224, 224))
sensor.skip_frames(time = 100)
clock = time.clock()

kpu = KPU()
kpu.load_kmodel("/sd/KPU/mnist/uint8_mnist_cnn_model.kmodel")   #模型参数

while True:
    gc.collect()
    img = sensor.snapshot()
    img_mnist1=img.to_grayscale(1)
    img_mnist2=img_mnist1.resize(112,112)
    img_mnist2.invert()
    img_mnist2.strech_char(1)
    img_mnist2.pix_to_ai()

    out = kpu.run_with_output(img_mnist2, getlist=True)

    # 动态计算中心点
    center_x = img.width() // 2
    center_y = img.height() // 2
    img.draw_cross(center_x, center_y)

    max_mnist = max(out)
    index_mnist = out.index(max_mnist)
    score = KPU.sigmoid(max_mnist)

    # 更新计数器
    count_dict[index_mnist] += 1

    if index_mnist == 0:
        if score == 1:
            display_str = "num: %d" % index_mnist
            print(display_str, score)
            img.draw_string(4,3,display_str,color=(0,0,0),scale=2)
    elif index_mnist == 1:
        if score > 0.15:
            display_str = "num: %d" % index_mnist
            print(display_str, score)
            img.draw_string(4,3,display_str,color=(0,0,0),scale=2)
    elif index_mnist == 4:
        if score == 1:
            display_str = "num: %d" % index_mnist
            print(display_str, score)
            img.draw_string(4,3,display_str,color=(0,0,0),scale=2)
    elif index_mnist == 5:
        if score > 0.999:
            display_str = "num: %d" % index_mnist
            print(display_str, score)
            img.draw_string(4,3,display_str,color=(0,0,0),scale=2)
    elif index_mnist == 7:
        if score > 0.999:
            display_str = "num: %d" % index_mnist
            print(display_str, score)
            img.draw_string(4,3,display_str,color=(0,0,0),scale=2)
    elif index_mnist == 8:
        if score > 0.999:
            display_str = "num: %d" % index_mnist
            print(display_str, score)
            img.draw_string(4,3,display_str,color=(0,0,0),scale=2)
    else:
        display_str = "num: %d" % index_mnist
        print(display_str, score)
        img.draw_string(4,3,display_str,color=(0,0,0),scale=2)

    lcd.display(img)

    # 2秒间隔
    current_time = time.ticks_ms()
    if time.ticks_diff(current_time, last_send_time) >= interval:
        max_count = 0
        max_digit = 0
        for digit, count in count_dict.items():
            if count > max_count:
                max_count = count
                max_digit = digit

        # 发送出现次数最多的数字
        send_data(max_digit)
        uart.send_bytearray(send_buf)

        count_dict = {i: 0 for i in range(10)}
        last_send_time = current_time

kpu.deinit()
