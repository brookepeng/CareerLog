#include "TimePoint.h"
#include <sstream>
#include <iomanip>
#include <regex>
#include <stdexcept>

// 构造函数实现
TimePoint::TimePoint(int h, int m, int s) : hour(h), minute(m), second(s) {}

// 静态工厂方法实现
TimePoint TimePoint::fromString(const std::string& str) {
    // 正则表达式：匹配 HH:MM:SS
    std::regex timePattern(R"((\d{1,2})[:．:](\d{2})[:．:](\d{2}))");
    std::smatch matches;

    if (std::regex_search(str, matches, timePattern)) {
        int h = std::stoi(matches[1].str());
        int m = std::stoi(matches[2].str());
        int s = std::stoi(matches[3].str());

        if (h >= 0 && h <= 23 && m >= 0 && m <= 59 && s >= 0 && s <= 59) {
            return TimePoint(h, m, s);
        }
        else {
            throw std::invalid_argument("时间数值非法");
        }
    }

    throw std::invalid_argument("无法识别的时间格式");
}

// 输出为字符串实现
std::string TimePoint::toString() const {
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << hour << ":"
        << std::setfill('0') << std::setw(2) << minute << ":"
        << std::setfill('0') << std::setw(2) << second;
    return oss.str();
}