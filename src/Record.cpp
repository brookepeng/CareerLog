#include "Record.h"
#include <sstream>
#include <stdexcept>

// 构造函数实现
Record::Record(const Date& d, const TimePoint& t, const std::string& info)
    : date(d), time(t), info(info) {}

// 静态工厂实现
Record Record::fromString(const std::string& str) {
    std::istringstream iss(str);
    std::string dateStr, timeStr;

    // 1. 提取日期和时间部分（假设它们内部无空格）
    if (!(iss >> dateStr >> timeStr)) {
        throw std::invalid_argument("记录格式错误：缺少日期或时间部分");
    }

    // 2. 委托给 Date 和 TimePoint 的工厂方法进行解析
    // 如果格式不对，异常会自动抛出
    Date d = Date::fromString(dateStr);
    TimePoint t = TimePoint::fromString(timeStr);

    // 3. 提取剩余的所有内容作为信息
    std::string info;
    std::getline(iss, info); // 读取剩余行

    // 去除左侧多余空格
    size_t start = info.find_first_not_of(" \t");
    if (start != std::string::npos) {
        info = info.substr(start);
    }
    else {
        info = ""; // 如果没有信息，设为空串
    }

    return Record(d, t, info);
}

// 输出为字符串实现
std::string Record::toString() const {
    std::ostringstream oss;
    oss << date.toString() << " "
        << time.toString() << " "
        << info;
    return oss.str();
}

const std::string& Record::getInformation() const {
    return info;
}

std::ostream& operator<<(std::ostream& os, const Record& rec) {
    os << rec.toString() << std::endl; // 打印内容并换行
    return os; // 返回流引用，支持链式调用 (如 cout << r1 << r2)
}
