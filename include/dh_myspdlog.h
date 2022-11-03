
#include <iostream>
#include <vector>
#include <thread>

#include "dh_instance.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"

namespace HCM_TOOL{

    // logger type
    typedef enum LoggerType{
        DAILY_LOGGER = 0,
        RORATING_LOGGER,
        COUNT,
    }LoggerType;

class MySpdlog : public HCM_TOOL::Instance<MySpdlog>{
//因为基类在GetInstance()中 new T() 时会调用到MySpdlog的构造函数
friend class HCM_TOOL::Instance<MySpdlog>;
friend class std::shared_ptr<MySpdlog>;

private:
    MySpdlog();
    ~MySpdlog();

    public:
        void SetLogPath(std::string path);
        void SetLogLevel(short level);
        void SetRotatingMaxSize(int rotating_max_size);
        void SetRotatingMaxFiles(int rotating_max_files);

    public:
        bool ExecLog(LoggerType lt);
        std::shared_ptr<spdlog::logger> GetLogger();
    
    private:
        // daily logger configure
        std::shared_ptr<spdlog::logger> DailyCfg();
        // rorating logger configure
        std::shared_ptr<spdlog::logger> RoratingCfg();

    private:
        // all logger var
        std::string _log_path;
        short _log_level;

        // rotating var
        int _rotating_max_size;
        int _rotating_max_files;

    private:
        std::shared_ptr<spdlog::logger> _logger;

// 可以通过函数来封装宏,实现只向控制台输出或者只向日志文件或者都输出.
#define SPDTRACE(...) SPDLOG_LOGGER_TRACE(spdlog::default_logger_raw(), __VA_ARGS__);SPDLOG_LOGGER_TRACE(MySpdlog::GetInstance()->GetLogger().get(), __VA_ARGS__)
#define SPDDEBUG(...) SPDLOG_LOGGER_DEBUG(spdlog::default_logger_raw(), __VA_ARGS__);SPDLOG_LOGGER_DEBUG(MySpdlog::GetInstance()->GetLogger().get(), __VA_ARGS__)
#define SPDINFO(...) SPDLOG_LOGGER_INFO(spdlog::default_logger_raw(), __VA_ARGS__);SPDLOG_LOGGER_INFO(MySpdlog::GetInstance()->GetLogger().get(), __VA_ARGS__)
#define SPDWARN(...) SPDLOG_LOGGER_WARN(spdlog::default_logger_raw(), __VA_ARGS__);SPDLOG_LOGGER_WARN(MySpdlog::GetInstance()->GetLogger().get(), __VA_ARGS__)
#define SPDERROR(...) SPDLOG_LOGGER_ERROR(spdlog::default_logger_raw(), __VA_ARGS__);SPDLOG_LOGGER_ERROR(MySpdlog::GetInstance()->GetLogger().get(), __VA_ARGS__)
#define SPDCRITICAL(...) SPDLOG_LOGGER_CRITICAL(spdlog::default_logger_raw(), __VA_ARGS__);SPDLOG_LOGGER_CRITICAL(MySpdlog::GetInstance()->GetLogger().get(), __VA_ARGS__)



//spd 带行号的打印，获取每日日志器并设置控制台为默认日志器,使其同时输出console和文件
//这些宏实际上是对daily_logger进行操作
// #define SPDTRACE(...) SPDLOG_LOGGER_TRACE(spdlog::default_logger_raw(), __VA_ARGS__);SPDLOG_LOGGER_TRACE(spdlog::get("daily_logger"), __VA_ARGS__)
// #define SPDDEBUG(...) SPDLOG_LOGGER_DEBUG(spdlog::default_logger_raw(), __VA_ARGS__);SPDLOG_LOGGER_DEBUG(spdlog::get("daily_logger"), __VA_ARGS__)
// #define SPDINFO(...) SPDLOG_LOGGER_INFO(spdlog::default_logger_raw(), __VA_ARGS__);SPDLOG_LOGGER_INFO(spdlog::get("daily_logger"), __VA_ARGS__)
// #define SPDWARN(...) SPDLOG_LOGGER_WARN(spdlog::default_logger_raw(), __VA_ARGS__);SPDLOG_LOGGER_WARN(spdlog::get("daily_logger"), __VA_ARGS__)
// #define SPDERROR(...) SPDLOG_LOGGER_ERROR(spdlog::default_logger_raw(), __VA_ARGS__);SPDLOG_LOGGER_ERROR(spdlog::get("daily_logger"), __VA_ARGS__)
// #define SPDCRITICAL(...) SPDLOG_LOGGER_CRITICAL(spdlog::default_logger_raw(), __VA_ARGS__);SPDLOG_LOGGER_CRITICAL(spdlog::get("daily_logger"), __VA_ARGS__)

// #define SPDDEBUG(...) SPDLOG_LOGGER_DEBUG(spdlog::default_logger_raw(), __VA_ARGS__);
// #define SPDINFO(...) SPDLOG_LOGGER_INFO(spdlog::default_logger_raw(), __VA_ARGS__);
// #define SPDWARN(...) SPDLOG_LOGGER_WARN(spdlog::default_logger_raw(), __VA_ARGS__);
// #define SPDERROR(...) SPDLOG_LOGGER_ERROR(spdlog::default_logger_raw(), __VA_ARGS__);
// #define SPDCRITICAL(...) SPDLOG_LOGGER_CRITICAL(spdlog::default_logger_raw(), __VA_ARGS__);

// #define SPDDEBUG(...) SPDLOG_LOGGER_DEBUG(spdlog::get("daily_logger"), __VA_ARGS__)
// #define SPDINFO(...) SPDLOG_LOGGER_INFO(spdlog::get("daily_logger"), __VA_ARGS__)
// #define SPDWARN(...) SPDLOG_LOGGER_WARN(spdlog::get("daily_logger"), __VA_ARGS__)
// #define SPDERROR(...) SPDLOG_LOGGER_ERROR(spdlog::get("daily_logger"), __VA_ARGS__)
// #define SPDCRITICAL(...) SPDLOG_LOGGER_CRITICAL(spdlog::get("daily_logger"), __VA_ARGS__)
};

}