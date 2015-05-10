#include <stacktrace.h>
#include <execinfo.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>

int main() {
    try {
        void *handle = dlopen(".libs/libtest_dlopen_exc.so", RTLD_LAZY);
        if (!handle) {
            std::cerr << "Cannot open library: " << dlerror() << std::endl;
            return 1;
        }

        typedef void (*foo_t)();
        foo_t foo = (foo_t)dlsym(handle, "foo");
        if (!foo) {
            std::cerr << "Cannot load symbol: " << dlerror() << std::endl;
            dlclose(handle);
            return 1;
        }

        foo();
    } catch (...) {
        void **trace;
        int trace_count = stacktrace_get_excbt(&trace);
        char **symbols = backtrace_symbols(trace, trace_count);
        for (int i = 2; i < trace_count; i++) {
            if (symbols != NULL) {
                std::vector<std::string> symbol_tokens;
                std::istringstream symbol_ss(symbols[i]);
                std::copy(std::istream_iterator<std::string>(symbol_ss),
                          std::istream_iterator<std::string>(),
                          back_inserter(symbol_tokens));
                printf("#%d %p - %s in %s\n",
                       (i - 2),
                       trace[i],
                       symbol_tokens[1].c_str(),
                       symbol_tokens[0].c_str());
            } else {
                printf("#%d %p\n", (i - 2), trace[i]);
            }
        }
        if (symbols != NULL) {
            free(symbols);
        }
    }
    return 0;
}
