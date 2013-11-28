#ifndef HARPE_ANALYSER_HPP
#define HARPE_ANALYSER_HPP

#include <mgf/Driver.hpp>

#include <iostream>

namespace harpe
{
    class Analyser
    {
        public:
            Analyser(const Analyser&) = delete;
            Analyser& operator=(const Analyser&) = delete;

            Analyser(std::istream& input);
            //Analyser(ntw::SocketSerialied& sock);
            
            int analyse();

            int analyse_next();

        protected:

        private:
            mgf::Driver driver;
    };
}
#endif
