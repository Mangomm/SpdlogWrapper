#include <iostream>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mutex>
#include <sys/time.h>
#include <map>
#include <type_traits>
#include "dh_global.h"
#include "dh_myspdlog.h"
using namespace HCM_TOOL;
std::string         HCM_TOOL::g_ProjectPath;

#define CONFIG_NAME "config.ini"

bool GetAppPathA(char* processdir, size_t len, string &excuDir){

	string p;
	int pos;
	p.clear();
	memset(processdir,0x00,len);
	excuDir.clear();
	size_t s = readlink("/proc/self/exe", processdir, len);
    if(s <= 0){
        return false;
    }
	//printf("get current path is :%s\n", processdir);//processdir = /xxx/main/main
    p = processdir;
	pos = p.find_last_of('/');
	if(std::string::npos == pos){
		return false;
	}
	excuDir = p.substr(0, pos + 1);//去除/xxx/main/main的main,得到可执行程序的目录
    return true;
}

int test_daily(){
    char tBuf[256] = {0};
    string cfgPath;
    int ret = GetAppPathA(tBuf, 256, cfgPath);
    if (ret == false) {
        printf("GetAppPathA is failed, path: {}.", cfgPath.c_str());
		return -1;
	}
    g_ProjectPath = cfgPath + "../";
    string cfgDir = g_ProjectPath + CONFIG_NAME;

    // 最好先获取对应的单例
    MySpdlog *insMysqdlog = MySpdlog::GetInstance();
    if(!insMysqdlog){
        printf("get logIns failed, program exit.\n");
        return -1;
    }

    insMysqdlog->SetLogPath(g_ProjectPath);
    insMysqdlog->SetLogLevel(0);
    insMysqdlog->ExecLog(DAILY_LOGGER);

    SPDINFO("info");
    SPDDEBUG("debug");
    SPDTRACE("trace");
    SPDDEBUG("Debug");
}

int test_rotating(){
    char tBuf[256] = {0};
    string cfgPath;
    int ret = GetAppPathA(tBuf, 256, cfgPath);
    if (ret == false) {
        printf("GetAppPathA is failed, path: {}.", cfgPath.c_str());
		return -1;
	}
    g_ProjectPath = cfgPath + "../";
    string cfgDir = g_ProjectPath + CONFIG_NAME;

    // 最好先获取对应的单例
    MySpdlog *insMysqdlog = MySpdlog::GetInstance();
    if(!insMysqdlog){
        printf("get logIns failed, program exit.\n");
        return -1;
    }

    insMysqdlog->SetLogPath(g_ProjectPath);
    insMysqdlog->SetLogLevel(2);
    insMysqdlog->SetRotatingMaxSize(1024*10);
    insMysqdlog->SetRotatingMaxFiles(2);
    insMysqdlog->ExecLog(RORATING_LOGGER);

    int i = 0;
    while(true){
        SPDINFO("{}", i++);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

}


int main(){

    test_daily();
    // test_rotating();

    return 0;
}