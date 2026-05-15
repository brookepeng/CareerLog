#include "Group.h"
#include <stdexcept> // 用于 std::out_of_range 异常
#include <iostream>
#include <algorithm>

// 1. 工厂构造函数实现
Group::Group(const std::string& name) : name_(name) {
    // records_ 向量会通过其默认构造函数自动初始化为空
}

// 2. 插入单个 Record 的方法实现
void Group::addRecord(const std::string& recordString) {
    // 直接利用 Record 的静态工厂方法 fromString 来解析字符串
    // 假设 recordString 格式为 "HH:MM:SS 信息内容"
    try {
        Record rec = Record::fromString(recordString);
        records_.push_back(rec);
        std::sort(records_.begin(), records_.end());
        
    } catch (const std::exception& e) {
        throw std::runtime_error("无法解析记录: " + recordString);
    }
}

// 3. 删除单个 Record 的方法实现
void Group::removeRecord(int index) {
    // 边界检查：确保索引有效
    if (index < 0 || index >= static_cast<int>(records_.size())) {
        throw std::out_of_range("记录编号超出范围");
    }
    // 使用 vector 的 erase 方法删除指定位置的元素
    // records_.begin() + index 指向要删除的元素
    records_.erase(records_.begin() + index);
}

// 4. getter 方法实现
const std::vector<Record>& Group::getRecords() const {
    return records_;
}

const std::string& Group::getName() const {
    return name_;
}

std::ostream& operator<<(std::ostream& os, const Group& group) {
    os << "\t=== Group: " << group.name_ << " ===" << std::endl; // 可选：打印组头

    // 遍历 vector，依次打印每个 Record
    // 注意：这里复用了上面定义的 Record 的 << 运算符
    for (const auto& record : group.records_) {
        os << "\t" << record;
    }
    os << "\t-------------------" << std::endl;
    return os;
}