#pragma once

#ifndef _WIN32
#include <link.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include <execinfo.h>
#endif

#include <vector>
#include <sstream>
#include <iostream>
#include <typeindex>


#define TRACE_MAX 1024

#ifndef NDEBUG
#define DBG_LOG_ENTER() TraceLogger __TraceLogger__(__FUNCTION__);
#else
#define DBG_LOG_ENTER()
#endif

namespace mvlt
{
    void PrintStackTrace();

    class TraceLogger
    {
    private:
        std::string FuncName;
    public:
        TraceLogger(const std::string& funcName);

        ~TraceLogger();
    };
}