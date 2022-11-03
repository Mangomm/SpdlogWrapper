project(spdlog_test)
cmake_minimum_required(VERSION 2.8)

# 判断编译器类型,如果是gcc编译器,则在编译选项中加入c++11支持,以下三者都可以
if(CMAKE_COMPILER_IS_GNUCXX)
    # add_compile_options编译C代码有警告
    add_compile_options(-std=c++11)
    # CMAKE_CXX_STANDARD未试过
    # set(CMAKE_CXX_STANDARD 11)
    # CMAKE_CXX_FLAGS编译C代码没警告(指编译器的警告,并非日志警告)
    # set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
    message(STATUS "optional:-std=c++11")   
endif(CMAKE_COMPILER_IS_GNUCXX)

#加载自定义模块
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_BUILD_TYPE Debug)
message(STATUS "编译类型:${CMAKE_BUILD_TYPE}")

# 设置linux、windows不同编译的方式
if(UNIX)
    set(CMAKE_C_COMPILER "${TOOLCHAIN_PREFIX}gcc")
    set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PREFIX}g++")
        if(CMAKE_BUILD_TYPE MATCHES "Debug")
            set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -g -ggdb3 -Wall -std=c++11 -fPIC")
            #有些场合加上-O0编译会报错
            #set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -g -ggdb3 -Wall -std=c++11 -fPIC")
            message(STATUS "CMAKE_CXX_FLAGS_DEBUG:${CMAKE_CXX_FLAGS_DEBUG}")
        else(CMAKE_BUILD_TYPE MATCHES "Debug")
            # 默认是release，不需要再设置
        endif(CMAKE_BUILD_TYPE MATCHES "Debug")
elseif(WIN32)
    # 给DEBUG、RELEASE设置对应的编译模式
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MTd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MTd")
endif()

#设置工程源码根目录，类似ZLM的做法，无需为每个目录下添加CMakeLists
#若要添加目录，这里对应set多一个路径，然后下面的file收集进来即可。并且本地的头文件直接先包含进来。
set(local_include ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${local_include})
set(app ${CMAKE_CURRENT_SOURCE_DIR}/app)
set(misc ${CMAKE_CURRENT_SOURCE_DIR}/misc)


# 设置默认的编译选项，想要改变，可在cmake ..时，添加额外的编译选项，例如cmake -DENABLE_MYSQL=ON .. 
option(ENABLE_SPDLOG "Enable SPDLOG" true)

#查找spdlog是否安装
find_package(SPDLOG QUIET)
if (SPDLOG_FOUND AND ENABLE_SPDLOG)
    message(STATUS "found include:${SPDLOG_INCLUDE_DIR},ENABLE_SPDLOG defined")
    include_directories(${SPDLOG_INCLUDE_DIR})
endif ()


# 上面已经找到对应的头文件目录以及库文件目录，并且头文件目录是包含进去了。
# 所以此时需要收集源文件.可以使用AUX_SOURCE_DIRECTORY、set、file进行收集，这里使用file。
# for file, detail see https://my.oschina.net/fonddream/blog/4596553.
file(GLOB src_list ${local_include}/*.h ${local_include}/*.hpp  ${app}/*.cpp ${misc}/*.cpp)

# 生成可执行文件及设置输出目录
ADD_EXECUTABLE(${PROJECT_NAME} ${src_list})
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# 添加库，去掉main.cpp再生成(不去也可以)，这一步实际上是可选的
# 指定库生成路径ARCHIVE_OUTPUT_PATH(应该不是ARCHIVE_OUTPUT_DIRECTORY)、LIBRARY_OUTPUT_PATH、RUNTIME_OUTPUT_PATH(应该不是RUNTIME_OUTPUT_DIRECTORY)
list(REMOVE_ITEM src_list ${app}/main.cpp)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
add_library(test_api SHARED ${src_list})
set_target_properties(test_api PROPERTIES VERSION 11.3 SOVERSION 0)

#pthread
TARGET_LINK_LIBRARIES(${PROJECT_NAME} pthread)

#自己的库同样可以链进来，不链也行
TARGET_LINK_LIBRARIES(${PROJECT_NAME} test_api)

MESSAGE(STATUS "PROJECT_NAME is: ${PROJECT_NAME}")

