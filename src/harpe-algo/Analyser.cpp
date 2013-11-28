#include <harpe-algo/Analyser.hpp>

#include <mgf/Analyse.hpp>

namespace harpe
{
    
    Analyser::Analyser(std::istream& input) : driver(input)
    {
    }

    int Analyser::analyse()
    {
        mgf::Analyse a = driver.parse();
        if (driver.isValid())
        {
        }
    }
}
