#include "Date.h"
#include <sstream>
#include <iomanip>
#include <regex>
#include <stdexcept>

// 构造函数实现
Date::Date(int y, int m, int d) : year(y), month(m), day(d) {}

// 静态工厂方法实现
Date Date::fromString(const std::string& str) {
    // 正则表达式：匹配 YYYY-MM-DD, YYYY/MM/DD 等格式
    // (\d{4}) 捕获年份
    // [-/.]   匹配分隔符
    // (\d{1,2}) 捕获月/日
    std::regex datePattern(R"((\d{4})[-/.](\d{1,2})[-/.](\d{1,2}))");
    std::smatch matches;

    if (std::regex_search(str, matches, datePattern)) {
        int y = std::stoi(matches[1].str());
        int m = std::stoi(matches[2].str());
        int d = std::stoi(matches[3].str());

        // 简单校验
        if (m >= 1 && m <= 12 && d >= 1 && d <= 31) {
            return Date(y, m, d);
        }
        else {
            throw std::invalid_argument("日期数值非法");
        }
    }

    throw std::invalid_argument("无法识别的日期格式");
}

// 输出为字符串实现
std::string Date::toString() const {
    std::ostringstream oss;
    oss << year << "-"
        << std::setfill('0') << std::setw(2) << month << "-"
        << std::setfill('0') << std::setw(2) << day;
    return oss.str();
}