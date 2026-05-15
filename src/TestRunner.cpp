#include "TestRunner.h"
#include "Date.h"
#include "TimePoint.h"
#include "Record.h"
#include "Group.h"
#include "Database.h"

#include <stdexcept>
#include <vector>
#include <string>
#include <iostream>

// 初始化全局统计变量
TestStats globalStats;

// ==========================================
// 1. Date 和 TimePoint 测试
// ==========================================

void test_Date_and_TimePoint() {
    std::cout << "\n--- 开始测试 Date 和 TimePoint ---" << std::endl;

    // --- 测试 Date ---
    try {
        //Date d(2023, 5, 9);
        Date d = Date::fromString("2023-05-09");
        ASSERT_EQUAL(d.getYear(), 2023, "Date Year");
        //ASSERT_EQUAL(d.getMonth(), 5, "Date Month");
        //ASSERT_EQUAL(d.getDay(), 9, "Date Day");
        ASSERT_EQUAL(d.toString(), "2023-05-09", "Date Format");
    }
    catch (TestFailureException&) {}

    // --- 测试 TimePoint ---
    try {
        //TimePoint t(8, 30, 0);
        TimePoint t = TimePoint::fromString("08:30:00");
        ASSERT_EQUAL(t.getHour(), 8, "TimePoint Hour");
        //ASSERT_EQUAL(t.getMinute(), 30, "TimePoint Minute");
        //ASSERT_EQUAL(t.getSecond(), 0, "TimePoint Second");
        ASSERT_EQUAL(t.toString(), "08:30:00", "TimePoint Format");
    }
    catch (TestFailureException&) {}

    // --- 测试异常 ---
    //ASSERT_THROW(Date(2023, 13, 1), std::invalid_argument, "Invalid Date Month");
    ASSERT_THROW(Date::fromString("2023-13-1"), std::invalid_argument, "Invalid Date Month");
}

// ==========================================
// 2. Record 测试
// ==========================================

void test_Record() {
    std::cout << "\n--- 开始测试 Record ---" << std::endl;

    // --- 标准格式 ---
    try {
        std::string input = "2023-05-09 08:30:00 系统启动";
        Record rec = Record::fromString(input);
        ASSERT_EQUAL(rec.toString(), input, "Standard Record Parse");
        ASSERT_EQUAL(rec.getInfo(), "系统启动", "Record Info Extraction");
    }
    catch (TestFailureException&) {}

    // --- 混合分隔符 ---
    try {
        std::string input = "2023/12/25 14:05:30 用户登录";
        Record rec = Record::fromString(input);
        ASSERT_EQUAL(rec.toString(), "2023-12-25 14:05:30 用户登录", "Mixed Separator Normalization");
    }
    catch (TestFailureException&) {}

    // --- 异常输入 ---
    ASSERT_THROW(Record::fromString("Bad Format"), std::invalid_argument, "Invalid Record Input");
}

// ==========================================
// 3. Group 测试
// ==========================================

void test_Group() {
    std::cout << "\n--- 开始测试 Group ---" << std::endl;

    // --- 构造与名称 ---
    try {
        Group myGroup("系统日志");
        ASSERT_EQUAL(myGroup.getName(), "系统日志", "Group Name");
        ASSERT_EQUAL(myGroup.getRecords().size(), (size_t)0, "Empty Group Size");
    }
    catch (TestFailureException&) {}

    // --- 添加记录 ---
    try {
        Group logGroup("应用日志");
        logGroup.addRecord("2023-06-15 09:00:00 程序启动");
        logGroup.addRecord("2023-06-15 09:01:30 用户登录");

        const auto& records = logGroup.getRecords();
        ASSERT_EQUAL(records.size(), (size_t)2, "Add Records Count");
        ASSERT_EQUAL(records[0].getInfo(), "程序启动", "First Record Content");
    }
    catch (TestFailureException&) {}

    // --- 删除记录 ---
    try {
        //Group taskGroup("待办事项");
        //taskGroup.addRecord("开会"); // 简化输入，假设 Record 能处理无前缀的情况或这里仅作演示
        //taskGroup.addRecord("吃饭");
        //taskGroup.addRecord("写报告");

        // 注意：这里为了演示删除逻辑，假设前面的 addRecord 成功
        // 实际运行时请确保 Record::fromString 能正确解析这些简略字符串，或者使用完整时间格式
        // 这里我们重新用完整格式测试一次以确保稳健：
        Group g2("G2");
        g2.addRecord("2023-01-05 01:00:00 A");
        g2.addRecord("2023-01-01 02:00:00 B");
        g2.addRecord("2023-01-06 03:00:00 C");
        std::cout << g2;
        g2.removeRecord(1); // 删除 B
        std::cout << g2;
        ASSERT_EQUAL(g2.getRecords().size(), (size_t)2, "Remove Record Count");
        ASSERT_EQUAL(g2.getRecords()[1].getInfo(), "C", "Remaining Record After Remove");
    }
    catch (TestFailureException&) {}

    // --- 删除异常 ---
    ASSERT_THROW({
        Group emptyGroup("空");
        emptyGroup.removeRecord(0);
        }, std::out_of_range, "Remove from Empty Group");
}

void test_Database() {
    std::cout << "\n--- 开始测试 Database 类 ---" << std::endl;

    // --- 测试 1: 构造与初始状态 ---
    {
        Database db;
        ASSERT_EQUAL(db.getGroups().size(), static_cast<size_t>(0), "新建数据库应为空");
    }

    // --- 测试 2: 添加组 (addGroup) ---
    {
        Database db;

        db.addGroup("系统日志");
        db.addGroup("用户操作");
        db.addGroup("错误报告");
        std::cout << db;
        const auto& groups = db.getGroups();
        ASSERT_EQUAL(groups.size(), static_cast<size_t>(3), "添加三个组后的数量");

        // 检查组的名称顺序
        ASSERT_EQUAL(groups[0].getName(), "系统日志", "第一个组的名称");
        ASSERT_EQUAL(groups[1].getName(), "用户操作", "第二个组的名称");
        ASSERT_EQUAL(groups[2].getName(), "错误报告", "第三个组的名称");
    }

    // --- 测试 3: 删除组 (removeGroup) ---
    {
        Database db;
        db.addGroup("A组");
        db.addGroup("B组");
        db.addGroup("C组");

        // 删除中间的组 (索引 1，即 "B组")
        db.removeGroup(1);

        const auto& remainingGroups = db.getGroups();
        ASSERT_EQUAL(remainingGroups.size(), static_cast<size_t>(2), "删除一个组后的数量");

        // 验证剩下的组是否正确衔接
        ASSERT_EQUAL(remainingGroups[0].getName(), "A组", "删除后保留的第一个组");
        ASSERT_EQUAL(remainingGroups[1].getName(), "C组", "删除后保留的第二个组");
    }

    // --- 测试 4: 删除组的异常处理 ---
    {
        Database db;
        db.addGroup("唯一组");

        bool exceptionThrown = false;

        try {
            // 尝试删除不存在的索引（越界）
            db.removeGroup(5);
        }
        catch (const std::out_of_range&) {
            exceptionThrown = true;
        }

        ASSERT_EQUAL(exceptionThrown, true, "删除越界索引应抛出 out_of_range 异常");

        // 再次测试：从空数据库中删除
        Database emptyDb;
        exceptionThrown = false;
        try {
            emptyDb.removeGroup(0);
        }
        catch (const std::out_of_range&) {
            exceptionThrown = true;
        }
        ASSERT_EQUAL(exceptionThrown, true, "从空数据库删除应抛出异常");
    }

    // --- 测试 5: 辅助方法 findGroupIndexByName ---
    {
        Database db;
        db.addGroup("日志");
        db.addGroup("数据");

        int index = db.findGroupIndexByName("数据");
        ASSERT_EQUAL(index, 1, "查找存在的组名称索引");

        int notFoundIndex = db.findGroupIndexByName("不存在的组");
        ASSERT_EQUAL(notFoundIndex, -1, "查找不存在的组名称应返回 -1");
    }

    std::cout << "--- Database 类测试结束 ---" << std::endl;
}

// ==========================================
// 统一入口函数实现
// ==========================================

void runAllTests() {
    std::cout << "========================================" << std::endl;
    std::cout << "       开始运行单元测试套件" << std::endl;
    std::cout << "========================================" << std::endl;

    // 依次调用各个测试函数
    // 每个函数内部会捕获自己的断言异常，从而保证一个测试失败不会导致整个程序崩溃
    test_Date_and_TimePoint();
    test_Record();
    test_Group();
    test_Database();

    std::cout << "\n========================================" << std::endl;
    std::cout << "               测试结果汇总" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "通过: " << globalStats.passed << std::endl;
    std::cout << "失败: " << globalStats.failed << std::endl;
    std::cout << "========================================" << std::endl;
}