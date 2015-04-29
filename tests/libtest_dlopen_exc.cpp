#include <cstdio>
#include <stdexcept>
#include <stacktrace.h>

void bar () {
    throw std::exception();
}

extern "C" void foo() {
    bar();
}
