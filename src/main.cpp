#include "CommandProcessor.h" // 引用自定义头文件
#include "TestRunner.h" // 只需要引用测试模块的头文件
#include <Windows.h>

int run() {
    // 创建对象
    CommandProcessor processor;
    // 调用方法
    processor.run();
    return 0;
}

int main() {
    // 设置 Windows 控制台输出为 UTF-8 (代码页 65001)
    SetConsoleOutputCP(65001);

    run();
    
    // 调用封装好的测试函数
    //runAllTests();
    //test_Date_and_TimePoint();

    //test_Record();

    return 0;
}
