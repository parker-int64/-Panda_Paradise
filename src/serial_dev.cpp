// #include<iostream>
// //#include<../headers/serial.hpp>
// #include"Python.h"

// void test_data(int send_num)
// { 
//     //int send_num = (rand()%2);
//     //Python环境地址
//     //Py_SetPythonHome(L"D:/Program Files/Python");

//     //初始化Python所需的环境
//     Py_Initialize();    
//     if (!Py_IsInitialized()){
//         std::cerr << "Unable to intialize the python enviroment!" << std::endl;
//         return ;
//     }

//     //引入当前路径,否则下面模块不能正常导入
//     PyRun_SimpleString("import sys");
//     PyRun_SimpleString("sys.path.append('./')");  //引入当前路径

//     PyObject *pModule = PyImport_ImportModule("serial_dev");
//     if(!pModule){
//         std::cerr << "Unable to fetch module serial_dev.\n";
//         return ;
//     }

//     PyObject *pDict = PyModule_GetDict(pModule);
//     if(!pDict){
//         std::cerr <<"Unable to fetch the dictionary.\n";
//         return ;
//     }

//     //直接获取模块中的函数
//     PyObject *pFunc = PyObject_GetAttrString(pModule, "send_data");
//     if(!pFunc){
//         std::cerr << "Unable to fetch function send_data().\n";
//         return ;
//     }
//     PyObject *pResult=NULL;
//     PyObject *pArgs = Py_BuildValue("(i)",send_num);
//     pResult=PyObject_CallObject(pFunc,pArgs);
//     if(!pResult){
//         std::cerr <<"Function error" << std::endl;
//         return ;
//     }
//     Py_Finalize(); //结束python虚拟机

// }

// void send_data(int send_num)
// {
//     //初始化Python所需的环境
//     Py_Initialize();  
//     Py_SetPythonHome(L"D:/Program Files/Python");  
//     if (!Py_IsInitialized()){
//         std::cerr << "Unable to intialize the python enviroment!" << std::endl;
//         return ;
//     }

//     //引入当前路径,否则下面模块不能正常导入
//     PyRun_SimpleString("import sys");
//     PyRun_SimpleString("sys.path.append('./')");  //引入当前路径

//     PyObject *pModule = PyImport_ImportModule("serial_dev");
//     if(!pModule){
//         std::cerr << "Unable to fetch module serial_dev.\n";
//         return ;
//     }

//     PyObject *pDict = PyModule_GetDict(pModule);
//     if(!pDict){
//         std::cerr <<"Unable to fetch the dictionary.\n";
//         return ;
//     }

//     //直接获取模块中的函数
//     PyObject *pFunc = PyObject_GetAttrString(pModule, "send_data");
//     if(!pFunc){
//         std::cerr << "Unable to fetch function send_data().\n";
//         return ;
//     }
//     PyObject *pResult=NULL;
//     PyObject *pArgs = Py_BuildValue("(i)",send_num);
//     pResult=PyObject_CallObject(pFunc,pArgs);
//     if(!pResult){
//         std::cerr <<"Function error" << std::endl;
//         return ;
//     }
//     Py_Finalize(); //结束python虚拟机
// }

// // int main()
// // {
// //     test_data(send_num);   //测试用，会产生随机数0/1发送给下位机
// //     send_data(send_num);
// //     return 0;
// // }