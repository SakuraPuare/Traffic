project(src)

aux_source_directory(. source)

# 把源码添加进来参与编译
add_executable(${PROJECT_NAME}_run ${source})
add_library(${PROJECT_NAME} ${source})
# 定义需要暴露的头文件
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})