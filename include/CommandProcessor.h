#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <functional> // 用于 std::function (可选，这里主要用函数指针)
#include "Database.h"
#include "ConfigManager.h"
#include "DataFileManager.h"

class CommandProcessor {
private:
    Database database_;
    Group* currentGroup_; // 当前上下文：nullptr 表示在 Database 层级
    bool flag_quit_ = false; //退出标志
    ConfigManager config_;
    DataFileManager datafile_;


    // --- 1. 定义成员函数指针类型 ---
    // 返回值 void，参数为 const std::vector<std::string>& (命令行参数)
    using CmdHandler = void (CommandProcessor::*)(const std::vector<std::string>&);

    // --- 2. 定义指令映射表 ---
    std::map<std::string, CmdHandler> db_commands_;    // Database 层级指令集
    std::map<std::string, CmdHandler> group_commands_; // Group 层级指令集

    // 辅助工具
    std::vector<std::string> split(const std::string& s);
    void printHelp();

public:
    CommandProcessor();

    // 初始化指令表
    void initCommands();

    // 主循环
    void run();

    // --- 3. 具体的业务处理方法 (声明) ---
    // Database 层级方法
    void cmdAddGroup(const std::vector<std::string>& args);
    void cmdRemoveGroup(const std::vector<std::string>& args);
    void cmdEnterGroup(const std::vector<std::string>& args);
    void cmdSaveToFile(const std::vector<std::string>& args);
    void cmdImportMarkdown(const std::vector<std::string>& args);

    // Group 层级方法
    void cmdAddRecord(const std::vector<std::string>& args);
    void cmdRemoveRecord(const std::vector<std::string>& args);
    void cmdExitGroup(const std::vector<std::string>& args);

    // 通用方法 (两个层级都可能用到)
    void cmdPrint(const std::vector<std::string>& args);
    void cmdHelp(const std::vector<std::string>& args);
    void cmdQuit(const std::vector<std::string>& args);
};

#endif