#include "Assertions.h"

#include <iostream>
#include <cli/utils.h>

void __assertion_failed(const char* val, const char* file, unsigned line, const char* func)
{
    std::cout << U::esc(ESC_ERROR) << "assertion failed: " << val << std::endl;
    std::cout <<                      "in " << file << ":" << line << " (" << func << ")" << U::esc(ESC_RESET) << std::endl;
}
