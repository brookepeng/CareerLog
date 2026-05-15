#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <string>

class ConfigManager {
private:
    std::string data_file_path_; 

public:
    // 默认构造函数，内部会使用写死的路径去读取配置
    ConfigManager();
    
    // 接口名同步更新，保持语义一致
    const std::string& getDataFilePath() const;
};

#endif // CONFIG_MANAGER_H