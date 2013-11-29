#ifndef HARPE_ANALYSER_HPP
#define HARPE_ANALYSER_HPP

#include <mgf/Driver.hpp>
#include <mgf/Spectrum.hpp>

#include <harpe-algo/Sequence.hpp>

#include <iostream>
#include <list>
#include <vector>

namespace harpe
{
    class Analyser
    {
        public:
            Analyser(const Analyser&) = delete;
            Analyser& operator=(const Analyser&) = delete;

            /**
             * \brief Construct a Analyser from stream
             * \param input MGF input stream
             */
            Analyser(std::istream& input);
            //Analyser(ntw::SocketSerialied& sock);
            
            /**
             * \brief analyse all the MS/MS spectrum in the parsed input, and try to build the Sequence
             */
            int analyse();

            /**
             * \brief analyse all the MS/MS spectrum in the parsed input, and try to build the Sequence
             */
            std::list<harpe::Sequence> analyse(mgf::Spectrum& s,int debut=-1);

        protected:

            const std::vector<int> get_index_max_intensitee_vector(mgf::Spectrum& spectrum,const int nb)const;
            

        private:
            mgf::Driver driver;
    };
}
#endif
