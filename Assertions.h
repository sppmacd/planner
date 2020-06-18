// *** ADDED BY HEADER FIXUP ***
#include <cstdlib>
#include <istream>
#include <ostream>
// *** END ***
#pragma once

#include <iostream>

void __assertion_failed(const char* val, const char* file, unsigned line, const char* func);

#define ASSERT_INFO(x,info) do {if(!(x)) {__assertion_failed(#x,__FILE__,__LINE__,__func__); std::cout << "info: " << info << std::endl; exit(1);}} while(0)
#define ASSERT(x) ASSERT_INFO(x,"none")
