#include "BackTrace.h"

namespace mvlt
{
    thread_local std::vector<std::string> Trace;

    void PrintStackTrace()
    {
        #ifdef _WIN32

        Trace.pop_back();
        std::size_t counter = Trace.size();
        for (auto traceIt = Trace.rbegin(); traceIt != Trace.rend(); ++traceIt)
        {
            std::cout << "\t" << counter << ": \x1B[33m" << *traceIt << "\033[0m in (File name and file line not available on windows)" << std::endl;
            --counter;
        }

        #else

        void* array[TRACE_MAX];
        size_t size;

        size = backtrace(array, TRACE_MAX);

        for (std::size_t i = 3; i < size; ++i)
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
                std::cout << "\t" << Trace.size() << ": \x1B[33m" << Trace.back() << "\033[0m in " << std::flush;
                Trace.pop_back();
            }
            else std::cout << "\t" << std::flush;
            
            std::string command =  "addr2line -e " + std::string(buf) + " " + sstream.str();
            system(command.c_str());
        }

        #endif
    }

    TraceLogger::TraceLogger(const std::string& funcName) : FuncName(funcName)
    {
        Trace.emplace_back(FuncName);
    }

    TraceLogger::~TraceLogger()
    {
        if (FuncName == Trace.back()) Trace.pop_back();
    }
}