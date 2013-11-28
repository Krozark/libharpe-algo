#include <harpe-algo/Analyser.hpp>

#include <mgf/Analyse.hpp>
#include <mgf/Spectrum.hpp>

namespace harpe
{
    
    Analyser::Analyser(std::istream& input) : driver(input)
    {
    }

    int Analyser::analyse()
    {
        mgf::Analyse analyse = driver.parse();
        if (driver.isValid())
        {
            auto spectrums(analyse.getSpectrums());
            for (mgf::Spectrum* spectrum : spectrums)
            {
            }
        }
    }
}
