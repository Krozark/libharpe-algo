#include <harpe-algo/Context.hpp>

#include <harpe-algo/defines.hpp>
#include <iostream>
#include <string>


namespace harpe
{
    AATab Context::aa_tab;
    double Context::error=0.05;
    int Context::finds_max_size=1000;
    Context::calc_score_type Context::calc_score_func = nullptr;

    #ifdef _WIN32 //_WIN64
    HMODULE Context::lib;
    FARPROC Context::initializer = nullptr;
    #elif __linux
    void* Context::lib = nullptr;
    void* Context::initializer = nullptr;
    #endif


    bool Context::loadFromLib(const std::string& libname)
    {
        bool res = false;
        #ifdef _WIN32 //_WIN64
        lib = LoadLibrary(libname.c_str());
        if (lib == nullptr)
        {
            HARPE_ALGO_ERROR("Enable to load "<<libname);
        }
        else
        {
            initializer = GetProcAddress(lib,"calc_score");
            if (initializer == nullptr)
            {
                HARPE_ALGO_ERROR("Enable to find calc_score() in "<<libname);
            }
            else
            {
                calc_score_func = (calc_score_type)(initializer);
                res = true;
            }
        }
        #elif __linux //|| __unix //or __APPLE__ 
        lib = dlopen(libname.c_str(), RTLD_LAZY);
        if (lib == nullptr)
        {
            HARPE_ALGO_ERROR("Enable to load "<<libname);
        }
        else
        {
            initializer = dlsym(lib,"calc_score");
            if (initializer == nullptr)
            {
                HARPE_ALGO_ERROR("Enable to find calc_score() in "<<libname);
            }
            else
            {
                std::cout<<(int)initializer<<std::endl;
                calc_score_func = (calc_score_type)(initializer);
                res = true;
            }
        }
        #endif
        return res;
    };

    void Context::closeLib()
    {
        #ifdef _WIN32 //_WIN64
        FreeLibrary(lib);
        #elif __linux
        dlclose(lib);
        #endif
    }

    double Context::calc_score(const double* const values)
    {
        return (*calc_score_func)(values);
    };
}
