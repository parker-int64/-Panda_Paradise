import sys
import serial
import random
import requests
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# in coord.txt: [2689.79,3218.99,5100.21]
# 串口传输
file = open("../data/coord3d.txt","r")
coord_lines=file.readline()
if(len(coord_lines) != 0):
    tmp_coord_x=coord_lines.split(",",2)[0] # x坐标中间变量（str）
    tmp_coord_y=coord_lines.split(",",2)[1] # y坐标中间变量（str）
    tmp_coord_z=coord_lines.split(",",2)[2] # z坐标中间变量（str）
    coord_x=float(tmp_coord_x)  # x的坐标（float）
    coord_y=float(tmp_coord_y)  # y的坐标（float）
    coord_z=float(tmp_coord_z)  # z的坐标（float）

else:
    print("Invalid coordinates received!")
print(coord_x)
print(coord_y)
print(coord_z)
print(coord_lines)


# 串口传输还未做测试
def transfer(PORT_NAME,BAUD_RATE,coord_lines):
    port = serial.Serial(PORT_NAME,BAUD_RATE,timeout=0.1)
    if  not port.isOpen():
        print("Error:port open failed!")
    if (len(coord_lines) !=0): #做判断，当数组长度不为0的时候才向串口发送数据
        port.write(str(coord_lines).encode())


def plot_figure(coord_x,coord_y,coord_z):
    fig = plt.figure()
    ax = Axes3D(fig)
    # 初始化x，y，z的值为空
    axx = []
    axy = []
    axz = []
    if (len(coord_lines) != 0):      
        axx.append(coord_x)             
        axy.append(coord_y)            
        axz.append(coord_z)
        ax.plot3D(axx,axy,axz)  
        plt.draw()             
        plt.pause(0.05)
    plt.savefig("./data/figure1.png")

for i in range(100):
    plot_figure(coord_x,coord_y,coord_z)
    # coord_x+=1.11
    # coord_y+=1.11
    # coord_z+=1.11



