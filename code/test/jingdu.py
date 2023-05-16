#########################
#用于检测串口并记录输出，并生成散点图
#使用了matplotlib、numpy、pyserial等python库，需要加载usbserial模块 `sudo modprobe usbserial`
#PySerial库来读取串口数据，matplotlib库来绘制散点图
#########################
import serial
import matplotlib.pyplot as plt
import warnings
import numpy as np
# 设置串口参数
ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)

# 定义空列表存储距离数据
distances = []

# 读取100次距离数据
for i in range(100):
    # 从串口读取一行数据
    line = ser.readline().decode().strip()
    
    # 检查是否为有效数据
    if line.startswith("测量距离 (mm) = "):
        # 解析距离数值并添加到列表中
        distance = float(line.replace("测量距离 (mm) = ", ""))
        distances.append(distance)

# 关闭串口连接
ser.close()

# 计算平均值和标准差
mean = np.mean(distances)
std = np.std(distances)

# 设置字体MiSans
plt.rcParams['font.sans-serif'] = ['MiSans']

# 调整y轴范围
plt.ylim(min(distances)-10, max(distances)+10)

# 绘制散点图，并设置点的大小为5
plt.scatter(range(1, len(distances)+1), distances, s=5)

# 绘制平均值和标准差的虚线
plt.axhline(y=mean, color='green', linestyle='--', label='平均值')
plt.axhline(y=mean+std, color='red', linestyle='--', label='标准差')
plt.axhline(y=mean-std, color='red', linestyle='--')

# 添加图例
plt.legend()

plt.title("100次距离测量")
plt.xlabel("测量次数")
plt.ylabel("距离 (mm)")
# 保存散点图为png图片
plt.savefig('distance_scatter.png', dpi=300)
plt.show()