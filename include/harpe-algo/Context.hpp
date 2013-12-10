#ifndef HARPE_CONTEXT_HPP
#define HARPE_CONTEXT_HPP

#include <harpe-algo/AATab.hpp>

#ifdef _WIN32 //_WIN64
    #include <windows.h>
#elif __linux //|| __unix //or __APPLE__ 
    #include <dlfcn.h>
#else
    #error not defined for this platform
#endif

namespace harpe
{
    class Context
    {
        public:
            Context() = delete;
            Context(const Context&) = delete;
            Context& operator=(const Context&) = delete;
            
            static bool loadFromLib(const std::string& libname);
            static void closeLib(); 

            static double calc_score(const double* const values);

            static AATab aa_tab;
            static double error;
            static int finds_max_size;

            typedef double (*calc_score_type)(const double* const values);
        private:
            static calc_score_type calc_score_func;
            #ifdef _WIN32 //_WIN64
            static HMODULE lib;
            static FARPROC initializer;
            #elif __linux
            static void* lib;
            static void* initializer;
            #endif

            
    };
}
#endif
