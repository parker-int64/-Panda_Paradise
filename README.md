# 基于OpenCV的颜色识别

## 环境说明

1. OpenCV 4.2，如果是OpenCV 3.x需要更改部分参数。
2. MacOS High Sierra
3. 编译器clang++ v10.0
4. mac只能在主线程中调用OpenCV的按键操作，所以需要在`main.cpp`中注释掉9-11行，并取消第14-15行的注释。
5. 默认识别的颜色是绿色，可以再UI界面调参保存后修改参数。

### 使用cvui动态调参

运行方式：

```SHELL
cd build
cmake ..
make -j4
./main
```

成功运行后会显示如下：

![运行界面](https://github.com/parker-int64/A-color-detection-demo/raw/master/data/running.png)
设置好参数后，可以点击Sava Parameters按钮来保存参数至`data/settings.yml`,然后可以根据参数来修改源程序的值。  
后续工作：可以直接从settings.yml读取参数。  

UI功能介绍：

1. HSV参数调整(HSV Control Interface)
2. 中值滤波核大小(Blur Kernel Size)
3. 形态学开闭操作元素大小(MorphologyEx Kernel Size)
4. Canny阈值设置(Canny Threshold)
5. 轮廓寻找模式和寻找方法(Mode,Method）
6. 轮廓点大小取舍(< x will be ereased)
7. 显示过程图像（hsv图->滤波图->边缘检测图->轮廓图） (Show Debug Images)默认关闭，勾选即可打开

### 在release模式下使用

默认的cmake模式是DEBUG模式，如果调好了参数后，不想显示cvui界面，可在`CMakeLists.txt`中第4行去掉`-DDEBUG`。

### 特别感谢

cvui的作者以及cvui项目。

[地址](https://github.com/Dovyski/cvui/)

[cvui使用指南](https://dovyski.github.io/cvui/)

cvui是一个只需要OpenCV就可以使用的简易UI库。
