#include "BackTrace.h"

namespace mvlt
{
    /// Thread local variable to store functions call trace
    thread_local std::vector<std::string> Trace;

    BackTraceFormat TraceFormat = BackTraceFormat::Compact;

    void SetBackTraceFormat(const BackTraceFormat& traceFormat) noexcept
    {
        TraceFormat = traceFormat;
    }

#ifndef NO_BACKTRACE_SUPPORT

    void PrintBackTrace() noexcept
    {
        if (TraceFormat != BackTraceFormat::None)
        {
            void* array[TRACE_MAX];
            size_t traceSize;
            std::size_t i = Trace.size() + 2;

            traceSize = backtrace(array, TRACE_MAX);

            if (TraceFormat == BackTraceFormat::Compact)
            {
                traceSize = i + 1;
                Trace.push_back(Trace.front());
            }

            for (; i < traceSize; ++i)
            {
                Dl_info info;
                link_map* extra_info;
                dladdr1(array[i], &info, reinterpret_cast<void**>(&extra_info), RTLD_DL_LINKMAP);
                // codechecker_intentional [all] no need to optimize it
                info.dli_fbase = (void*)extra_info->l_addr;

                if (info.dli_sname == NULL)
                    info.dli_saddr = info.dli_fbase;

                ptrdiff_t offset = static_cast<char*>(array[i]) - static_cast<char*>(info.dli_saddr) - 1;

                char buf[BUFSIZ];

                // codechecker_intentional [all] suppress
                std::size_t readed = readlink("/proc/self/exe", buf, BUFSIZ);
                std::stringstream sstream;
                sstream << std::hex << offset;

                if (!Trace.empty())
                {
                    std::cout << "\t";
                    std::cout << "\x1B[33m" << Trace.back() << "\033[0m from " << std::flush;
                    Trace.pop_back();
                }
                else std::cout << "\t" << std::flush;

                std::string command = "addr2line -e " + std::string(buf) + " " + sstream.str();

                // codechecker_intentional [all] its not a problem to use system because it is not input from user and it is only for debug
                int res = system(command.c_str());
            }
        }
    }

#else

    void PrintBackTrace() noexcept
    {
        if (TraceFormat != BackTraceFormat::None)
        {
            std::cout << "\t\x1B[33m" << Trace.front() 
                << "\033[0m from (File name and file line not available on this machine! If you are on linux try to download libexecinfo and reconfigure project. See Checkers/BackTrace)" << std::endl;
        }
    }

#endif

    TraceLogger::TraceLogger(const std::string& funcName) noexcept : FuncName(funcName)
    {
        Trace.emplace_back(FuncName);
    }

    TraceLogger::~TraceLogger() noexcept
    {
        if (FuncName == Trace.back()) Trace.pop_back();
    }
}