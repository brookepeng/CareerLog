#ifndef DATE_H
#define DATE_H

#include <string>

class Date {
private:
    int year;
    int month;
    int day;

    // 私有构造函数，强制使用静态工厂方法
    Date(int y, int m, int d);

public:
    // 静态工厂方法：从字符串创建对象
    static Date fromString(const std::string& str);

    // 成员方法：转换为字符串
    std::string toString() const;

    // Getter (可选，用于测试)
    int getYear() const { return year; }

    // 重载 < : 左操作数早于右操作数返回真
    bool operator<(const Date& d) const {
        if (year != d.year) return year < d.year;
        if (month != d.month) return month < d.month;
        return day < d.day;
    }

    // 重载 == : 用于判断是否相等
    bool operator==(const Date& d) const {
        return year == d.year && month == d.month && day == d.day;
    }
};

#endif // DATE_H