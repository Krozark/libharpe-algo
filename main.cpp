#include <iostream>
#include <fstream>

#include <mgf/Driver.hpp>
#include <harpe-algo/Analyser.hpp>
#include <harpe-algo/Context.hpp>

int main(int argc,char* argv[])
{
    if (not harpe::Context::loadFromLib("./calc_score.so"))
        return 1;

    harpe::Context::aa_tab.add("A",71.037110);
    harpe::Context::aa_tab.add("C",103.009185);
    harpe::Context::aa_tab.add("D",115.026943);
    harpe::Context::aa_tab.add("E",129.042593);
    harpe::Context::aa_tab.add("F",147.068414);
    harpe::Context::aa_tab.add("G",57.021464);
    harpe::Context::aa_tab.add("H",137.058912);
    harpe::Context::aa_tab.add("I-L",113.084064);
    harpe::Context::aa_tab.add("K",128.094963);
    harpe::Context::aa_tab.add("M",131.040485);
    harpe::Context::aa_tab.add("N",114.042927);
    harpe::Context::aa_tab.add("P",97.052764);
    harpe::Context::aa_tab.add("Q",128.058578);
    harpe::Context::aa_tab.add("R",156.101111);
    harpe::Context::aa_tab.add("S",87.032028);
    harpe::Context::aa_tab.add("T",101.047679);
    harpe::Context::aa_tab.add("V",99.068414);
    harpe::Context::aa_tab.add("W",186.079313);
    harpe::Context::aa_tab.add("Y",163.063320);

    harpe::Context::aa_tab.sort();

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
