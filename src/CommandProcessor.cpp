#include "CommandProcessor.h"
#include "Version.h"
#include <sstream>
#include <stdexcept>

CommandProcessor::CommandProcessor() : database_(),currentGroup_(nullptr) ,datafile_(config_.getDataFilePath())
{
    initCommands();
    
}

// 简单的字符串分割
std::vector<std::string> CommandProcessor::split(const std::string& s) {
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string token;
    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

// 初始化指令映射表
void CommandProcessor::initCommands() {
    // --- 注册 Database 层级指令 ---
    db_commands_["add_group"] = &CommandProcessor::cmdAddGroup;
    db_commands_["remove_group"] = &CommandProcessor::cmdRemoveGroup;
    db_commands_["enter"] = &CommandProcessor::cmdEnterGroup;
    db_commands_["print"] = &CommandProcessor::cmdPrint;
    db_commands_["help"] = &CommandProcessor::cmdHelp;
    db_commands_["save_to_file"] = &CommandProcessor::cmdSaveToFile;
    db_commands_["import_md"] = &CommandProcessor::cmdImportMarkdown;
    db_commands_["quit"] = &CommandProcessor::cmdQuit;

    // --- 注册 Group 层级指令 ---
    group_commands_["add_record"] = &CommandProcessor::cmdAddRecord;
    group_commands_["remove_record"] = &CommandProcessor::cmdRemoveRecord;
    group_commands_["exit"] = &CommandProcessor::cmdExitGroup;
    group_commands_["print"] = &CommandProcessor::cmdPrint;
    group_commands_["help"] = &CommandProcessor::cmdHelp;
    group_commands_["save_to_file"] = &CommandProcessor::cmdSaveToFile;
    group_commands_["quit"] = &CommandProcessor::cmdQuit;
}

void CommandProcessor::run() {
    std::cout << "=== 系统启动 ===" << std::endl;

    cmdImportMarkdown({"", datafile_.getFilePath()});

    std::string input;

    while (!flag_quit_) {
        // 1. 显示提示符
        if (currentGroup_ == nullptr) {
            std::cout << "[DB]> ";
        }
        else {
            std::cout << "[GRP:" << currentGroup_->getName() << "]> ";
        }

        // 2. 获取并解析输入
        if (!std::getline(std::cin, input)) break;
        if (input.empty()) continue;

        std::vector<std::string> tokens = split(input);
        std::string cmdKey = tokens[0];

        // 3. 根据当前状态选择指令表并进行查找 (动态派发)
        std::map<std::string, CmdHandler>* currentMap = nullptr;

        if (currentGroup_ == nullptr) {
            currentMap = &db_commands_;
        }
        else {
            currentMap = &group_commands_;
        }

        auto it = currentMap->find(cmdKey);
        if (it != currentMap->end()) {
            // 找到指令，调用对应的成员函数
            // 语法：(this->*函数指针)(参数)
            try {
                (this->*(it->second))(tokens);
            }
            catch (const std::exception& e) {
                std::cerr << "错误: " << e.what() << std::endl;
            }
        }
        else {
            std::cerr << "未知命令: " << cmdKey << std::endl;
        }
    }

    cmdSaveToFile({"",datafile_.getFilePath()});
    std::cout<<"程序已正常退出"<<std::endl;
}

// ==========================================
// 具体业务逻辑实现
// ==========================================

void CommandProcessor::cmdAddGroup(const std::vector<std::string>& args) {
    if (args.size() < 2) throw std::runtime_error("用法: add_group <名称>");
    database_.addGroup(args[1]);
    std::cout << "组 '" << args[1] << "' 已添加" << std::endl;
}

void CommandProcessor::cmdRemoveGroup(const std::vector<std::string>& args) {
    if (args.size() < 2) throw std::runtime_error("用法: remove_group <ID>");
    int id = std::stoi(args[1]);
    database_.removeGroup(id);
    std::cout << "组 ID " << id << " 已删除" << std::endl;
}

void CommandProcessor::cmdEnterGroup(const std::vector<std::string>& args) {
    if (args.size() < 2) throw std::runtime_error("用法: enter <ID>");
    int id = std::stoi(args[1]);

    const auto& groups = database_.getGroups();
    if (id < 0 || id >= static_cast<int>(groups.size())) {
        throw std::out_of_range("组 ID 不存在");
    }

    currentGroup_ = const_cast<Group*>(&groups[id]);
    std::cout << "已进入组: " << currentGroup_->getName() << std::endl;
}

void CommandProcessor::cmdAddRecord(const std::vector<std::string>& args) {
    if (args.size() < 4) throw std::runtime_error("用法: add_record <日期> <时间> <信息>");
    // 简单拼接信息部分
    std::string info = "";
    for (size_t i = 3; i < args.size(); ++i) info += args[i] + " ";

    std::string recordStr = args[1] + " " + args[2] + " " + info;
    currentGroup_->addRecord(recordStr);
    std::cout << "记录已添加" << std::endl;
}

void CommandProcessor::cmdRemoveRecord(const std::vector<std::string>& args) {
    if (args.size() < 2) throw std::runtime_error("用法: remove_record <ID>");
    int id = std::stoi(args[1]);
    currentGroup_->removeRecord(id);
    std::cout << "记录 ID " << id << " 已删除" << std::endl;
}

void CommandProcessor::cmdExitGroup(const std::vector<std::string>& args) {
    if (currentGroup_ != nullptr) {
        currentGroup_ = nullptr;
        std::cout << "已返回 Database 层级" << std::endl;
    }
    else {
        std::cout << "当前已在 Database 层级" << std::endl;
    }
}

void CommandProcessor::cmdPrint(const std::vector<std::string>& args) {
    if (currentGroup_ == nullptr) {
        std::cout << database_ << std::endl;
    }
    else {
        std::cout << *currentGroup_ << std::endl;
    }
}

void CommandProcessor::cmdHelp(const std::vector<std::string>& args) {
    // 打印版本信息
    std::cout << "CareerLog Version: " << APP_VERSION_STRING << std::endl; 
    std::cout << "------------------------" << std::endl;
    
    std::cout << "可用命令:\n";
    if (currentGroup_ == nullptr) {
        for (const auto& p : db_commands_) std::cout << "  " << p.first << "\n";
    }
    else {
        for (const auto& p : group_commands_) std::cout << "  " << p.first << "\n";
    }
}

void CommandProcessor::cmdSaveToFile(const std::vector<std::string> &args)
{
    if (args.size() < 2)
    {
        throw std::runtime_error("用法: save_to_file <文件名>");
    }

    std::string filename = args[1];
    // 可以简单检查是否以 .txt 或 .log 结尾，或者直接交给 Database 处理
    database_.exportToFile(filename);
}

void CommandProcessor::cmdImportMarkdown(const std::vector<std::string> &args)
{
    if (args.size() < 2)
    {
        throw std::runtime_error("用法: import_md <文件名>");
    }
    database_.importFromFile(args[1]);
}

void CommandProcessor::cmdQuit(const std::vector<std::string> &args)
{
    flag_quit_ = true;
}
