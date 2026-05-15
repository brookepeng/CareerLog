#ifndef DATA_FILE_MANAGER_H
#define DATA_FILE_MANAGER_H

#include <string>

class DataFileManager {
private:
    std::string file_path_;
    
    // 提取出的公共方法：检查路径，若文件不存在则创建空文件
    void ensureFileExists(const std::string& path);

public:
    explicit DataFileManager(const std::string& path);
    
    // 提供 setter 动态修改路径
    void setFilePath(const std::string& path);
    
    const std::string& getFilePath() const;
};

#endif // DATA_FILE_MANAGER_H