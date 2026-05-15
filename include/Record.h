#ifndef RECORD_H
#define RECORD_H

#include <string>
#include "Date.h"       // 需要包含依赖的头文件
#include "TimePoint.h"  // 需要包含依赖的头文件

class Record {
private:
    Date date;
    TimePoint time;
    std::string info;

    // 私有构造函数
    Record(const Date& d, const TimePoint& t, const std::string& info);

public:
    // 静态工厂函数：解析 "日期 时间 信息" 格式
    static Record fromString(const std::string& str);

    // 输出为字符串
    std::string toString() const;

    // Getter
    const std::string& getInformation() const;

    friend std::ostream& operator<<(std::ostream& os, const Record& rec);

    // 重载 < : 先比日期，若相等再比时间
    bool operator<(const Record& other) const {
        // 1. 如果当前日期早于对方日期，直接返回真
        if (date < other.date) {
            return true;
        }

        // 2. 如果当前日期晚于对方日期，返回假
        // (这里利用了 !(other.date < date) 来判断相等的情况)
        if (other.date < date) {
            return false;
        }

        // 3. 日期相等时，比较时间
        return time < other.time;
    }
};

#endif // RECORD_H