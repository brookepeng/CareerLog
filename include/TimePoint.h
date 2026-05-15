#ifndef TIMEPOINT_H
#define TIMEPOINT_H

#include <string>

class TimePoint {
private:
    int hour;
    int minute;
    int second;

    // 私有构造函数
    TimePoint(int h, int m, int s);

public:
    // 静态工厂方法
    static TimePoint fromString(const std::string& str);

    // 成员方法：转换为字符串
    std::string toString() const;

    // Getter
    int getHour() const { return hour; }

    // 重载 <
    bool operator<(const TimePoint& t) const {
        if (hour != t.hour) return hour < t.hour;
        if (minute != t.minute) return minute < t.minute;
        return second < t.second;
    }

    // 重载 ==
    bool operator==(const TimePoint& t) const {
        return hour == t.hour && minute == t.minute && second == t.second;
    }
};

#endif // TIMEPOINT_H