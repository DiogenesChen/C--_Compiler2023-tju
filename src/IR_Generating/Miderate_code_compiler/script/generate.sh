#!/bin/bash
# # -----------------------------------------------------------------------------
# # 判断系统是否为Linux
# if [ ! -n "${LINUX}" ]; then
#     echo "The current system is not Linux, please use Linux to run this script."
#     exit 1
# fi
# # -----------------------------------------------------------------------------
# 判断是否创建了build目录，且目录是否为空，如果存在非空build文件夹，则进入，否则创建
if [ -d "build" ]; then
    if [ ! -z "$(ls -A build)" ]; then
        cd build
    else
        mkdir build
        cd build
    fi
else
    mkdir build
    cd build
fi
# -----------------------------------------------------------------------------
# 重新执行cmake -Dtest=on .. 指令
cmake ..
# -----------------------------------------------------------------------------
# 根据机器的nproc判断最大线程数，如果返回结果大于10，则运行make -j10，否则运行make -j4
if [ $(nproc) -gt 10 ]; then
    make clean
    make -j10
else
    make clean
    make -j4
fi
# -----------------------------------------------------------------------------
# 告知运行成功，并提示用户使用方法，./project1 为运行主程序，./runUnitsTest 为运行单元
# 测试程序，输出内容为绿色
echo -e "\e[1;32mRun successfully!\e[0m"
echo -e "\e[1;32mUsage: ./project1 is run main program\e[0m"
echo -e "\e[1;32mUsage: ./runUnitTests is run unit test\e[0m"
