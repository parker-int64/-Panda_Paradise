#**接收UDP，并写入coord.yml(mini PC上)
# server:供mini PC读取并解析三维坐标
# mini PC解析完三维坐标后
# 1.通过pyserial传出
# 2.plotly画出3D图
import socket
import sys
#udp无连接服务器端
#SOCK_DGRAM UDP模式
while True:
    ip_port=("127.0.0.1",8000)
    s=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
    s.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
    s.bind(ip_port)
    recv_data=s.recv(1024)
    print(recv_data.decode())
    # TODO: 写入coord.yml(mini PC)
    
    