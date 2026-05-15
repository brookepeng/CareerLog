#include "ConfigManager.h"
#include <fstream>
#include <stdexcept>
#include <iostream>

ConfigManager::ConfigManager() {
    const std::string CONFIG_FILE = "config.txt";
    std::ifstream infile(CONFIG_FILE);
    
    if (!infile.is_open()) {
        throw std::runtime_error("配置文件不存在或无法打开: " + CONFIG_FILE);
    }

    if (std::getline(infile, data_file_path_)) {
        std::cout << "[配置] 成功读取数据文件路径: " << data_file_path_ << std::endl;
    } else {
        throw std::runtime_error("配置文件内容为空: " + CONFIG_FILE);
    }
    infile.close();
}

const std::string& ConfigManager::getDataFilePath() const {
    return data_file_path_;
}