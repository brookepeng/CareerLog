#include "DataFileManager.h"
#include <fstream>
#include <iostream>

void DataFileManager::ensureFileExists(const std::string& path) {
    std::ifstream infile(path);
    if (!infile.is_open()) {
        std::ofstream outfile(path);
        if (outfile.is_open()) {
            std::cout << "[路径管理器] 目标文件不存在，已自动创建空文件: " << path << std::endl;
            outfile.close();
        } else {
            std::cerr << "[路径管理器] 警告：无法创建目标文件: " << path << std::endl;
        }
    } else {
        infile.close();
    }
}

DataFileManager::DataFileManager(const std::string& path) {
    setFilePath(path);
}

void DataFileManager::setFilePath(const std::string& path) {
    ensureFileExists(path);
    file_path_ = path;
}

const std::string& DataFileManager::getFilePath() const {
    return file_path_;
}