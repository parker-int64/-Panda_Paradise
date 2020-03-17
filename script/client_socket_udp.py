#*---utf-8---*
# client: raspberry pi,发送数据
#发送coord.yml中生成的内容
#使用之前请chmod +x ./client_socket_udp.py
#!/usr/bin/env python
import socket
import sys
while True:
    f=open("../data/coord.yml","r")
    lines = f.readlines()
    last_line = lines[-1] # 最后一行
    if (len(last_line) != 0):
        print(last_line)
        ip_port=("127.0.0.1",8000)
        s=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
        s.sendto(last_line.encode(),ip_port)
        