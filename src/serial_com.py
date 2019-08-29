import serial
port = serial.Serial("COM4", baudrate=115200, timeout=0.1) #发送延时100ms
def send_data(a):
    port.write(str('%d'%a).encode())