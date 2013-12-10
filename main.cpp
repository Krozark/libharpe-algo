#include <iostream>
#include <fstream>

#include <mgf/Driver.hpp>
#include <harpe-algo/Analyser.hpp>
#include <harpe-algo/Context.hpp>

int main(int argc,char* argv[])
{
    if (harpe::Context::loadFromLib("./calc_score.so"))
        return 1;
    int r=0;
    std::ifstream file(argv[1], std::ifstream::in);
    if (file.good())
    {

        mgf::Driver driver(file);
        mgf::Spectrum* spectrum = nullptr;
        while((spectrum = driver.next()) != nullptr)
        {
            spectrum->__print__(std::cout);
            auto l = harpe::Analyser::analyse(*spectrum,-1);
            delete spectrum;
        }
        file.close();

    }
    harpe::Context::closeLib();
    return r;
}
