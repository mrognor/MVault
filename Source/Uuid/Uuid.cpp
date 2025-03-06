#include "Uuid.h"

namespace mvlt 
{
    std::string ToHex(std::uint64_t digit) noexcept
    {
        static constexpr char hex_chars[] = "0123456789abcdef";
        std::string res(16, '0');
    
        for (int i = 15; i >= 0; --i)
        {
            // Add to res last 4 bits
            res[i] = hex_chars[digit & 0xf];
            digit >>= 4;
        }
    
        return res;
    }
    
    std::string Uuid() noexcept
    {
        std::string res;
        std::random_device rd;
        std::uniform_int_distribution<std::uint64_t> dist(0, std::numeric_limits<std::uint64_t>::max());
    
        // Gen fully random 64 bit number
        std::string a = ToHex(dist(rd));
    
        // Gen random number with two first bits: 10. This is equal 1 uuid variant
        std::string b = ToHex((dist(rd) >> 2) | (1ull << 63));
    
        // xxxxxxxx
        res += a.substr(0, 8);
        res += "-";
    
        // xxxxxxxx-xxxx
        res += a.substr(8, 4);
        res += "-4";
    
        // xxxxxxxx-xxxx-4xxx
        res += a.substr(13, 3);
        res += "-";
    
        // xxxxxxxx-xxxx-4xxx-{89ab}xxx
        res += b.substr(0, 4);
        res += "-";
        
        // xxxxxxxx-xxxx-4xxx-{89ab}xxx-xxxxxxxxxxxx
        res += b.substr(4, 12);
    
        return res;
    }

    std::string GenTmpFileName(const std::string& prefix) noexcept
    {
        #ifndef _WIN32

        if (prefix.empty())
            return "/tmp/" + Uuid();
        else
            return "/tmp/" + prefix + Uuid();
        
        #else
        
        if (prefix.empty())
            return "C:\\Windows\\Temp\\" + Uuid();
        else
            return "C:\\Windows\\Temp\\" + prefix + Uuid();

        #endif
    }
}