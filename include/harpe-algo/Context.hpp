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
    /**
     * \brief A class that contain the global datas and param for the algorithm
     */
    class Context
    {
        public:
            Context() = delete;
            Context(const Context&) = delete;
            Context& operator=(const Context&) = delete;
            
            /**
             * \brief Load the sort algorithm of sequence form a exernal lib
             * This function have to be call befor any analyse.
             * \param libname the path of the lib
             * \return true if ok
             */
            static bool loadFromLib(const std::string& libname);

            /**
             * \brief proprely close the oppening lib
             */
            static void closeLib(); 

            /**
             * \brief cal the loaded function from the external lib.
             * loadFromLib have to be call before.
             * \param values The stats table of a sequence
             * \return the score
             */
            static double calc_score(const double* const values);

            static AATab aa_tab; ///< the table of AAs
            static double error; ///< the maximum error (in Daltons)
            static unsigned int finds_max_size; ///< the max number of Sequence to store [defined is 1000]
            static unsigned int finds_max_size_tmp; ///< the max number of Sequence to store during the algorithm (have to be greater tha finds_max_size [default is 5000])

        private:
            typedef double (*calc_score_type)(const double* const values); ///< the type of the calc_score function

            /**
             * \brief A union to store the calc_score function whene load
             */
            static union Alias{
                calc_score_type calc_score_func; ///< function
                void* obj; ///< the return type of the function loading function.
            } alias;   ///< store the calc_score function

            #ifdef _WIN32 //_WIN64
            static HMODULE lib; ///< lib manager
            #elif __linux
            static void* lib; ///< lig manager
            #endif
    };
}
#endif
