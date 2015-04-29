#include <stacktrace.h>
#include <iostream>
#include <dlfcn.h>

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
        struct stacktrace *trace = stacktrace_get_exc();
        if (trace != NULL) {
            stacktrace_print(trace);
        } else {
            std::cerr << "No exception backtrace\n" << std::endl;
        }
    }
    return 0;
}
