
include(CheckCXXSourceCompiles)

# SPDLOG是一个跨平台的库，所以这里不分平台也没事
# 只有头文件(实际内部也是包含很多类，只是写在.h而非cpp)，所以只需添加头文件路径即可，不需加库路径。

# 1 寻找头文件路径
if(WIN32)
   find_path(SPDLOG_INCLUDE_DIR spdlog/spdlog.h
      PATHS
      ${CMAKE_CURRENT_SOURCE_DIR}/3rdlib/spdlog/
      # 找不到路径，需要在这里配上对应的路径
   )
else(WIN32)
   find_path(SPDLOG_INCLUDE_DIR spdlog/spdlog.h
      PATHS
      ${CMAKE_CURRENT_SOURCE_DIR}/3rdlib/spdlog/
   )
endif(WIN32)

if(SPDLOG_INCLUDE_DIR)
   set(SPDLOG_FOUND TRUE)
else(SPDLOG_INCLUDE_DIR)
   set(SPDLOG_FOUND FALSE)
   message(STATUS "SPDLOG not found.")
endif(SPDLOG_INCLUDE_DIR)
