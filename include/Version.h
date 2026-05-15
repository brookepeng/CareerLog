#ifndef VERSION_H
#define VERSION_H

// --- 在这里设置版本号 ---
#define APP_VERSION_MAJOR 1   // 主版本号
#define APP_VERSION_MINOR 0   // 次版本号
#define APP_VERSION_PATCH 0   // 修订号

// 组合成字符串宏，方便打印
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define APP_VERSION_STRING TOSTRING(APP_VERSION_MAJOR) "." TOSTRING(APP_VERSION_MINOR) "." TOSTRING(APP_VERSION_PATCH)

#endif // VERSION_H