#ifndef GROUP_H
#define GROUP_H

#include <vector>
#include <string>
#include "Record.h" // 引入之前定义的 Record 类

class Group {
private:
    std::string name_;              // 组的名称
    std::vector<Record> records_;   // 存储 Record 对象的动态数组

public:
    // 1. 工厂构造函数
    explicit Group(const std::string& name);

    // 2. 插入单个 Record 的方法
    // 参数格式与 Record::fromString 相同
    void addRecord(const std::string& recordString);

    // 3. 删除单个 Record 的方法
    // 参数为编号（从0开始）
    void removeRecord(int index);

    // 4. vector<Record> 的 getter
    const std::vector<Record>& getRecords() const;

    // 额外提供一个获取组名的方法
    const std::string& getName() const;

    friend std::ostream& operator<<(std::ostream& os, const Group& group);
};

#endif // GROUP_H