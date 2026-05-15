#include "Database.h"
#include <stdexcept> // 用于 std::out_of_range
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

// 2. 添加 Group 的实现
void Database::addGroup(const std::string& name) {
    // 创建一个名为 name 的新 Group，并添加到 vector 末尾
    groups_.push_back(Group(name));
}

// 3. 删除 Group 的实现
void Database::removeGroup(int index) {
    // 边界检查
    if (index < 0 || index >= static_cast<int>(groups_.size())) {
        throw std::out_of_range("组编号超出范围");
    }
    // 使用 vector 的 erase 方法删除指定位置的元素
    groups_.erase(groups_.begin() + index);
}

// 4. Getter 的实现
const std::vector<Group>& Database::getGroups() const {
    return groups_;
}

// 5. 查找组的索引
int Database::findGroupIndexByName(const std::string& name) const {
    for (size_t i = 0; i < groups_.size(); ++i) {
        if (groups_[i].getName() == name) {
            return static_cast<int>(i);
        }
    }
    return -1; // 未找到
}

// 6. 重载输出流 <<
std::ostream& operator<<(std::ostream& os, const Database& db) {
    os << "=== 数据库概览 ===" << std::endl;
    os << "共有 " << db.groups_.size() << " 个组" << std::endl;

    int index = 0;
    for (const auto& group : db.groups_) {
        // 打印组头和分隔线
        os << "[" << index << "] " << group.getName() << std::endl;
        //os << "--------------------------" << std::endl;

        // 复用 Group 的 << 运算符来打印组内的所有记录
        // 注意：Group 的 << 已经包含了换行，所以这里不需要额外加 endl
        os << group;
        os << "--------------------------" << std::endl;
        index++;
    }
    return os;
}

void Database::exportToFile(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        throw std::runtime_error("无法打开文件 " + filename + " 进行写入");
    }

    // --- 生成当前时间戳 ---
    // 获取当前系统时间
    auto now = std::chrono::system_clock::now();
    std::time_t time_t = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&time_t);

    // 格式化时间为: YYYY-MM-DD 星期X
    std::ostringstream timeStream;
    timeStream << std::put_time(&tm, "%Y-%m-%d");
    std::string dateStr = timeStream.str();

    // 简单的数字转中文星期映射 (localtime 的 tm_wday: 0=周日, 1=周一...)
    const char* weekdays[] = {"周日", "周一", "周二", "周三", "周四", "周五", "周六"};
    std::string weekdayStr = weekdays[tm.tm_wday];

    // --- 写入 Markdown 头部 ---
    // 包含你要求的 <time_location> 标签
    outFile << "# 求职日志" << std::endl << std::endl;
    outFile << "<time_location>" << std::endl;
    outFile << dateStr << " " << weekdayStr << std::endl;
    outFile << "陕西省 西安市" << std::endl; // 这里写死为西安，如需动态修改可提取为变量
    outFile << "</time_adjust>" << std::endl << std::endl;

    // --- 写入统计信息 ---
    outFile << "- **总组数**: " << groups_.size() << std::endl;
    outFile << "- **导出时间**: " << std::put_time(&tm, "%H:%M:%S") << std::endl;
    outFile << std::endl;
    
    // 分隔线
    outFile << "---" << std::endl << std::endl;

    // --- 遍历所有 Group (作为二级标题) ---
    for (size_t i = 0; i < groups_.size(); ++i) {
        const Group& group = groups_[i];
        
        // 2. Group 名作为二级标题
        outFile << "## " << group.getName() << std::endl << std::endl;
        
        const auto& records = group.getRecords();
        if (records.empty()) {
            outFile << "暂无记录。" << std::endl << std::endl;
        } else {
            // --- 遍历所有 Record ---
            for (size_t j = 0; j < records.size(); ++j) {
                const Record& rec = records[j];

                // 3. Record 作为正文
                // 格式：- [时间] 信息
                // 例如：- [09:00:00] 投递简历
                outFile << "- **[" << rec.getDate().toString() << " " << rec.getTime().toString() << "]** "
                        << rec.getInfo() << std::endl;
            }
        }
        outFile << std::endl; // 组之间留空行
    }
    
    outFile.close();
    std::cout << "数据库已成功导出到文件: " << filename << std::endl;
}

void Database::importFromFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        throw std::runtime_error("无法打开文件 " + filename + " 进行读取");
    }

    std::string line;
    Group* currentGroup = nullptr; // 指向当前正在处理的组

    while (std::getline(inFile, line)) {
        // 去除行首尾的空白字符（可选，增加容错性）
        // 这里为了简洁假设格式严格，实际可写 trim 函数
        
        // 1. 检测是否为 Group 标题 (## GroupName)
        if (line.rfind("## ", 0) == 0) { // 以 "## " 开头
            // 提取组名 (去掉 "## " 前缀)
            std::string groupName = line.substr(3);
            
            // 创建新组
            addGroup(groupName);
            
            // 更新当前组指针，指向最新创建的组
            // 注意：这里假设 addGroup 是在 vector 末尾添加
            if (!groups_.empty()) {
                currentGroup = &groups_.back();
            }
        }
        // 2. 检测是否为 Record 列表项 (- **[HH:MM:SS]** 信息)
        else if (line.rfind("- **[", 0) == 0 && line.find("]**") != std::string::npos) {
            if (currentGroup == nullptr) {
                // 防御性编程：记录出现在组之前
                continue;
            }

            // 提取时间与信息
            // 示例行: - **[19:00:00]** 投递简历
            size_t timeStart = line.find('[') + 1;
            size_t timeEnd = line.find(']');
            size_t infoStart = line.find("** ", timeEnd) + 3; // 跳过 "** " 

            if (timeStart < timeEnd && infoStart < line.length()) {
                std::string timeStr = line.substr(timeStart, timeEnd - timeStart);
                std::string infoStr = line.substr(infoStart);

                // 拼接成 Record 可识别的格式： "HH:MM:SS 信息"
                std::string recordData = timeStr + " " + infoStr;

                // 使用 Group 的 addRecord 方法
                currentGroup->addRecord(recordData);
            }
        }
        // 3. 忽略其他行（如头部、分割线、空行）
    }

    inFile.close();
    std::cout << "数据库已成功从文件导入: " << filename << std::endl;
}