#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#ifndef UTILS_H
#define UTILS_H

#include "config.h"

#define DBGPRINT_VECTOR(v) do {for(auto& o: v) std::cout << o << ", " << std::endl;} while(0);

#define ESC_RESET 0
#define ESC_BOLD 1
#define ESC_ERROR 2

// utils
namespace U
{
    std::string concatenateArgs(std::vector<std::string>& args, int start);
    struct _Esc { std::string code; };

    std::string escapeCode(int n);

    // handle escape sequences
    inline _Esc
    esc(int __n)
    { return { escapeCode(__n) }; }

    template<typename _CharT, typename _Traits>
    inline std::basic_ostream<_CharT, _Traits>&
    operator<<(std::basic_ostream<_CharT, _Traits>& __os, _Esc __f)
    {
        return __os << __f.code;
    }
}

#endif // UTILS_H
