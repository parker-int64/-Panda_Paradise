#include<iostream>
#include<../headers/serial.hpp>
#include"Python.h"
void send_data(int send_num)
{

    Py_SetPythonHome(L"D:/Program Files/Python37");//地址
    Py_Initialize();    //初始化Python所需的环境
    if (!Py_IsInitialized()){
        std::cerr << "Unable to intialize the python enviroment!" << std::endl;
    }

    //引入当前路径,否则下面模块不能正常导入
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('../src/')");  //引入当前路径

    //测试用，引入模块2
    PyObject *pModule = PyImport_ImportModule("serial_com");
    if(!pModule){
        std::cerr << "Unable to fetch the serial_com.\n ";
    }

    //获取字典属性
    PyObject *pDict = PyModule_GetDict(pModule);
    if(!pDict){
        std::cerr <<"Unable to fetch the dictionary.\n";
    }

    //直接获取模块中的函数
    PyObject *pFunc = PyObject_GetAttrString(pModule, "send_data");
    if(!pFunc){
        std::cerr <<"Unable to find the function.\n";
    }
    PyObject *pResult=NULL;
     //(i)表示将C++中的整型变量转换为python中整型变量
    // 向python中的函数传参,这里只是测试，所以调用rand()产生随机数
        PyObject *pArgs = Py_BuildValue("(i)",send_num); 
        pResult=PyObject_CallObject(pFunc,pArgs);
    if(!pResult){
        std::cerr <<"Functions error" << std::endl;
        }

    Py_Finalize();  
    //note:如果把serial.py发送这一块写成函数，那么调用是否还需要获取类和实例呢？Yes
    //随机数test_data,通过传参的方式传给python。
    //测试1：运行代码，保证能够正常的运行（还有port.write()这一部分得看下）
    //测试2：传参测试。
}
// int main(){
//     send_data(0);
//     return 0;
// }
