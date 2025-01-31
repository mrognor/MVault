#include "BackTrace.h"

namespace mvlt
{
    /// Thread local variable to store functions call trace
    thread_local std::vector<std::string> Trace;

    BackTraceFormat TraceFormat = BackTraceFormat::Compact;

    void SetBackTraceFormat(const BackTraceFormat& traceFormat)
    {
        TraceFormat = traceFormat;
    }

    #ifndef _WIN32

    void PrintBackTrace()
    {
        if (TraceFormat != BackTraceFormat::None)
        {
            void* array[TRACE_MAX];
            size_t traceSize;
            std::size_t i = 3;

            traceSize = backtrace(array, TRACE_MAX);

            if (TraceFormat == BackTraceFormat::Compact)
            {
                i = Trace.size() + 2;
                traceSize = i + 1;
                Trace.push_back(Trace.front());
            }

            for (; i < traceSize; ++i)
            {
                Dl_info info;
                link_map *extra_info;
                dladdr1(array[i], &info, reinterpret_cast<void**>(&extra_info), RTLD_DL_LINKMAP);
                info.dli_fbase = (void *) extra_info->l_addr;

                if (info.dli_sname == NULL)
                    info.dli_saddr = info.dli_fbase;

                ptrdiff_t offset = static_cast<char*>(array[i]) - static_cast<char*>(info.dli_saddr) - 1;

                char buf[BUFSIZ];
                std::size_t readed = readlink("/proc/self/exe", buf, BUFSIZ);
                std::stringstream sstream;
                sstream << std::hex << offset;

                if (!Trace.empty())
                {
                    std::cout << "\t";
                    if (TraceFormat == BackTraceFormat::Full) std::cout << Trace.size() << ": ";
                    std::cout << "\x1B[33m" << Trace.back() << "\033[0m from " << std::flush;
                    Trace.pop_back();
                }
                else std::cout << "\t" << std::flush;
                
                std::string command =  "addr2line -e " + std::string(buf) + " " + sstream.str();
                system(command.c_str());
            }

        
        }
    }

    #else

    void PrintBackTrace()
    {
        if (TraceFormat != BackTraceFormat::None)
        {
            Trace.pop_back();
            std::size_t counter = Trace.size();
            for (auto traceIt = Trace.rbegin(); traceIt != Trace.rend(); ++traceIt)
            {
                std::cout << "\t" << counter << ": \x1B[33m" << *traceIt << "\033[0m from (File name and file line not available on windows)" << std::endl;
                --counter;
            }
        }
    }

    #endif

    TraceLogger::TraceLogger(const std::string& funcName) : FuncName(funcName)
    {
        Trace.emplace_back(FuncName);
    }

    TraceLogger::~TraceLogger()
    {
        if (FuncName == Trace.back()) Trace.pop_back();
    }
}