"""
检测红色色块 red_threshold（阈值）

波特率：115200
发送数据：X、色块x中心、Y、色块y中心、”；“
检验位：0
停止位：1
"""
import sensor, image, time, lcd
from modules import ybserial

# 初始化LCD
lcd.init(freq=15000000)
lcd.clear(lcd.RED)

# 初始化摄像头
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=1000)

# 初始化串口并配置参数
uart = ybserial(
    uart_num=1,          # UART 端口号 (1, 2, 3)
    baudrate=115200,     # 波特率
    bits=8,              # 数据位 (5, 6, 7, 8)
    parity='N',          # 校验位: 'N' (无), 'E' (偶校验), 'O' (奇校验)
    stop=1,              # 停止位
    timeout=1000         # 超时时间 (ms)
)

# 目标颜色阈值
red_threshold = (30, 70, 10, 70, 10, 70)

# fps
fps_counter = 0
fps = 0
last_time = time.ticks_ms()
fps_history = [0] * 5   #长度为5，5s
history_index = 0

# 目标丢失计数
lost_counter = 0

# 串口调试计数
send_count = 0
error_count = 0

while True:
    # 平滑（5s）
    fps_counter += 1    #1s fps计数
    current_time = time.ticks_ms()
    if current_time - last_time >= 1000:
        fps_history[history_index] = fps_counter
        history_index = (history_index + 1) % 5
        fps = sum(fps_history) // 5
        fps_counter = 0
        last_time = current_time

    # 图像捕获
    img = sensor.snapshot()

    # 色块识别
    blobs = img.find_blobs([red_threshold],
                           pixels_threshold=200,
                           area_threshold=200,
                           merge=True)

    if blobs:
        max_blob = max(blobs, key=lambda b: b.area())

        # 绘制目标标记
        img.draw_rectangle(max_blob.rect(), color=(0,255,0))
        img.draw_cross(max_blob.cx(), max_blob.cy(), color=(255,0,0))

         # 发送坐标给STM32
        data_str = "X{},Y{};".format(max_blob.cx(), max_blob.cy())
        result = uart.send(data_str)
        send_count += 1
        # 显示发送状态
        status = "SEND: {}".format(data_str)
        img.draw_string(10, 30, status, color=(0,255,0), scale=1.2)
    else:
        result = uart.send("X-1,Y-1;")#当没有检测到红色色块时，发送负坐标给STM32做丢目标处理
        send_count += 1
        img.draw_string(10, 10, "Target: LOST", color=(255,0,0), scale=1.2)

    #提示信息
    img.draw_string(10, 50, "Sent: {}".format(send_count), color=(0,255,255))
    img.draw_string(10, 70, "Errors: {}".format(error_count), color=(255,0,0))
    img.draw_string(10, 220, "FPS: %d" % fps, color=(0,255,255))
    img.draw_string(280, 10, "UART", color=(0,255,0), x_spacing=-1)  # 右侧显示
    img.draw_string(280, 30, "ON", color=(0,255,0), x_spacing=-1)    # 右侧显示

    # 连续10帧未检测到显示警告
    if lost_counter > 10:
        img.draw_string(80, 100, "SEARCHING...", color=(255,0,0), scale=2)

    # 显示图像中心标记（+）
    img.draw_cross(160, 120, color=(0,0,255), size=10)

    lcd.display(img)

    # 检查串口状态
    if result < 0:
        error_count += 1
        print("UART Error:", result)

    time.sleep_ms(50)
