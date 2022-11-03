#include "dh_myspdlog.h"
#include "dh_global.h"
using namespace HCM_TOOL;

MySpdlog::MySpdlog() : _log_path(""), _log_level(1), _rotating_max_size(1048576 * 30), _rotating_max_files(10){
}

MySpdlog::~MySpdlog(){
    spdlog::drop_all();        
}

void MySpdlog::SetLogPath(std::string path) {
	_log_path = path;
}

void MySpdlog::SetLogLevel(short level) {
	_log_level = level;
}

void MySpdlog::SetRotatingMaxSize(int rotating_max_size){
    _rotating_max_size = rotating_max_size;
}

void MySpdlog::SetRotatingMaxFiles(int rotating_max_files){
    _rotating_max_files = rotating_max_files;
}

std::shared_ptr<spdlog::logger> MySpdlog::DailyCfg(){
	std::string log;
    // set log path
	if (_log_path.empty()) {
		log = "daily-logs/daily.txt";
	}else {
		log = _log_path + "daily-logs/daily.txt";
	}
	
	return spdlog::daily_logger_mt("daily_logger", log, 0, 0);
}

std::shared_ptr<spdlog::logger> MySpdlog::RoratingCfg(){
	std::string log;
    // set log path
	if (_log_path.empty()) {
		log = "rotating-logs/rotating.txt";
	}else {
		log = _log_path + "rotating-logs/rotating.txt";
	}

	// Create a file rotating logger with xxxmb size max and xxx rotated files
	return spdlog::rotating_logger_mt("rotating_logger", log, _rotating_max_size, _rotating_max_files);
}

bool MySpdlog::ExecLog(LoggerType lt) {

    switch (lt)
    {
    case DAILY_LOGGER:
        _logger = DailyCfg();
        break;
    case RORATING_LOGGER:
        _logger = RoratingCfg();
        break;
    default:
        break;
    }

    if(nullptr == _logger){
        return false;
    }

    // Set global log level
    spdlog::set_level(static_cast<spdlog::level::level_enum>(_log_level));
    // Set single sink log level
    _logger.get()->set_level(static_cast<spdlog::level::level_enum>(_log_level));

	// 设置单个输出格式.
	// daily_logger.get()->set_pattern("%Y-%m-%d %H:%M:%S [%n] [%^ %l %$] [thread %t] - <%s>|<%#>|<%!>, %v");
	// 设置全局输出格式.
	// %n记录器名称.例如控制台,文件.
	// %^起始顏色范围(只能使用一次).%L消息的简短日志级别(建议用小写%l).%$:結束顏色范围(例如%^ [+++]%$%v)(只能使用一次)
	// %s：文件名.%#：行号. %!：函数名.
	//spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] --- [%^ %l %$] [t %t]--->>> %v");
	spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] --- [thread %t] [%#] [%^ %l %$] --->>> %v");

	// 设置当出现 _log_level 或 更严重的错误时立刻刷新日志到 disk 
	_logger->flush_on(static_cast<spdlog::level::level_enum>(_log_level));

    // 在dll程序会卡死？
    spdlog::flush_every(std::chrono::seconds(3));

	return true;
}

std::shared_ptr<spdlog::logger> MySpdlog::GetLogger(){
    return _logger;
}