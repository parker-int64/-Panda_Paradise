# 在branch下选择不同版本以查看更新日志
# -Panda_Paradise
2019年四川省机器人大赛熊猫乐园代码
·Created by Parker 

·Requirements: opencv 3 or later, python (pyserial)

·Use camera please uncomment the *cap.open(0)* in imageProcess.cpp.

·For linux user: serial_dev.py serial_dev.cpp (NOTE:change your serial port)

·For windows user: serial_com.py serial_com.cpp(NOTE:change your serial port)

·For linux user: use pkg-config to find your include files or library files.



说明

·直接git到本地目录，环境：opencv3及以后版本，调用python（pyserial）用于串口传输。

·如果想使用摄像头请请取消在imageProcess.cpp中cap.open(0)的注释。

·linux下请取消serial_dev.cpp的注释，并将serial_com.cpp的内容注释掉，更改自己的串口。

·windows相反。

·CMakeList请根据自己的情况修改opencv的头文件和库文件，还有python的头文件和库文件。

·Linux 用户可以使用`pkg-config --cflags opencv`和`pkg-config --libs opencv`来寻找opencv库。

