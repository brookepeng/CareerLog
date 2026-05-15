cmake_minimum_required(VERSION 3.10)
project(CareerLog VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17) # 使用 C++11 标准以支持 move 等特性

# 包含头文件目录（推荐使用 target_include_directories 代替全局的 include_directories）
include_directories(${PROJECT_SOURCE_DIR}/include)

# 收集源码和头文件
file(GLOB_RECURSE HEADER_FILES "${PROJECT_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# 生成可执行文件
add_executable(careerlog ${SOURCE_FILES} ${HEADER_FILES})

# 自动复制配置文件到可执行文件所在的目录
# 这样无论你的 careerlog.exe 在 build 目录的哪个子文件夹里，都能找到同级的 config.txt
add_custom_command(TARGET careerlog POST_BUILD
    # 复制 config.txt（如果存在）
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/config.txt
    $<TARGET_FILE_DIR:careerlog>
)

# 在 IDE 中按目录结构展示头文件
source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_FILES})
source_group(TREE "${PROJECT_SOURCE_DIR}/src" PREFIX "Source Files" FILES ${SOURCE_FILES})

# 设置默认启动项
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT
  ${PROJECT_NAME})

message("PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
message("SOURCE_FILES: ${SOURCE_FILES}")
