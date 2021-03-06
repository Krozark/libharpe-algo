#include <harpe-algo/Context.hpp>

#include <harpe-algo/defines.hpp>
#include <iostream>
#include <string>


namespace harpe
{
    AATab Context::aa_tab;
    double Context::error=0.05;
    int Context::max_charge = 0;
    unsigned int Context::finds_max_size=1000;
    unsigned int Context::finds_max_size_tmp=1000*5;
    Context::MOD Context::mod = Context::MOD::NORMAL;
    Context::Alias Context::alias;

    #ifdef _WIN32 //_WIN64
    HMODULE Context::lib;
    #elif __linux
    void* Context::lib = nullptr;
    #endif


    bool Context::loadFromLib(const std::string& libname)
    {
        bool res = false;
        #ifdef _WIN32 //_WIN64
        lib = LoadLibrary((LPCTSTR)libname.c_str());
        if (lib == nullptr)
        {
            HARPE_ALGO_ERROR("Enable to load "<<libname);
        }
        else
        {
            alias.obj = (void*)GetProcAddress(lib,"calc_score");
            if (alias.obj == nullptr)
            {
                HARPE_ALGO_ERROR("Enable to find calc_score() in "<<libname);
            }
            else
            {
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
            alias.obj = dlsym(lib,"calc_score");
            if (alias.obj == nullptr)
            {
                HARPE_ALGO_ERROR("Enable to find calc_score() in "<<libname);
            }
            else
            {
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
        return (*f())(values);
    };

    Context::calc_score_type Context::f()
    {
        return alias.calc_score_func;
    }
}
