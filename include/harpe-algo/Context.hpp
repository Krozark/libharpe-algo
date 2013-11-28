#ifndef HARPE_CONTEXT_HPP
#define HARPE_CONTEXT_HPP

#include <harpe-algo/AATab.hpp>

namespace harpe
{
    class Context
    {
        public:
            Context() = delete;
            Context(const Context&) = delete;
            Context& operator=(const Context&) = delete;
            
            static AATab aa_tab;


        protected:

        private:
    };
}
#endif
