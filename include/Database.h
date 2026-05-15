#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <string>
#include <iostream>
#include "Group.h" // 引入 Group 类

class Database {
private:
    std::vector<Group> groups_; // 存储所有 Group 对象的容器

public:
    // 1. 默认构造函数
    Database() = default;

    // 2. 添加 Group 的方法
    // 参数为组名，内部会构造一个新的空 Group
    void addGroup(const std::string& name);

    // 3. 删除 Group 的方法
    // 参数为组的索引（编号）
    void removeGroup(int index);

    // 4. 获取所有组的引用（用于遍历或测试）
    const std::vector<Group>& getGroups() const;

    // 5. 辅助方法：根据名称查找组的索引
    // 如果找不到返回 -1
    int findGroupIndexByName(const std::string& name) const;

    // 6. 重载输出流运算符 <<
    friend std::ostream& operator<<(std::ostream& os, const Database& db);
};

#endif // DATABASE_H