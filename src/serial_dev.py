import serial
port = serial.Serial("/dev/ttyAMA0", baudrate=115200, timeout=0.1) #发送延时100ms
def send_data(send_num):
    port.write(str('%d'%send_num).encode())