# CMakeList.txt: Compress 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# 将源代码添加到此项目的可执行文件。
add_executable (Compress "Compress.cpp" "Compress.h" "Test.cpp")

# 设置变量
SET(LIBCOMPRESS_SRC ./Compress.h ./Compress.cpp)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR}/lib_out)

# 第一个参数为你需要构建的dll的名字，第二个为类型
ADD_LIBRARY(compress_dll SHARED ${LIBCOMPRESS_SRC})
if(WIN32)
  install(TARGETS compress_dll
    RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/dll)
else()
  install(TARGETS compress_dll
    LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/dll)
endif()  

# 为dll设置linker
# 指定dll的生成目录，这里是：./lib_out
SET_TARGET_PROPERTIES(compress_dll PROPERTIES LINKER_LANGUAGE C
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        OUTPUT_NAME "compress_dll"
        PREFIX "")


# TODO: 如有需要，请添加测试并安装目标。
