#include <link.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include <execinfo.h>

int main()
{
    void* array[1024];
    std::size_t traceSize;

    traceSize = backtrace(array, 1024);

    Dl_info info;
    link_map* extra_info;
    dladdr1(array[0], &info, reinterpret_cast<void**>(&extra_info), RTLD_DL_LINKMAP);
    // codechecker_intentional [all] no need to optimize it
    info.dli_fbase = (void*)extra_info->l_addr;

    if (info.dli_sname == NULL)
        info.dli_saddr = info.dli_fbase;

    std::ptrdiff_t offset = static_cast<char*>(array[0]) - static_cast<char*>(info.dli_saddr) - 1;
}