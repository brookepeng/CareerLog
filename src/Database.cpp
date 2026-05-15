#include "Database.h"
#include <stdexcept> // 用于 std::out_of_range

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