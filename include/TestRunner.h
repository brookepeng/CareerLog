#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include <string>
#include <iostream>
#include <exception>

// --- 1. 声明各个模块的测试函数 ---
void test_Date_and_TimePoint();
void test_Record();
void test_Group();
void test_Database();

// --- 2. 简易测试框架的核心逻辑 ---

// 用于统计测试结果
struct TestStats {
    int passed = 0;
    int failed = 0;
};

// 全局统计变量（简单实现）
extern TestStats globalStats;

// 自定义异常：用于在断言失败时中断当前测试函数
class TestFailureException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Test Assertion Failed";
    }
};

// 核心断言宏：如果 a != b，则抛出异常
// __FILE__ 和 __LINE__ 会自动替换为当前的文件名和行号，方便定位错误
#define ASSERT_EQUAL(a, b, msg) \
    if ((a) != (b)) { \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ << " - " << msg << std::endl; \
        std::cerr << "       Expected: " << (b) << ", but got: " << (a) << std::endl; \
        throw TestFailureException(); \
    } else { \
        std::cout << "[PASS] " << msg << std::endl; \
        globalStats.passed++; \
    }

// 用于检查是否抛出特定异常的宏
#define ASSERT_THROW(statement, exception_type, msg) \
    try { \
        statement; \
        std::cerr << "[FAIL] " << msg << " (No exception thrown)" << std::endl; \
        globalStats.failed++; \
    } catch (const exception_type&) { \
        std::cout << "[PASS] " << msg << std::endl; \
        globalStats.passed++; \
    } catch (...) { \
        std::cerr << "[FAIL] " << msg << " (Wrong exception type)" << std::endl; \
        globalStats.failed++; \
    }

// --- 3. 统一入口函数 ---
void runAllTests();

#endif // TEST_RUNNER_H