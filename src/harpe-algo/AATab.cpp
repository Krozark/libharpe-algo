#include <harpe-algo/AATab.hpp>

#include <algorithm>

namespace harpe
{
    
    AATab::AATab()
    {
        ///\load from soket
        AAs.emplace_back("A",71.037110);
        AAs.emplace_back("C",103.009185);
        AAs.emplace_back("D",115.026943);
        AAs.emplace_back("E",129.042593);
        AAs.emplace_back("F",147.068414);
        AAs.emplace_back("G",57.021464);
        AAs.emplace_back("H",137.058912);
        AAs.emplace_back("I-L",113.084064);
        AAs.emplace_back("K",128.094963);
        AAs.emplace_back("M",131.040485);
        AAs.emplace_back("N",114.042927);
        AAs.emplace_back("P",97.052764);
        AAs.emplace_back("Q",128.058578);
        AAs.emplace_back("R",156.101111);
        AAs.emplace_back("S",87.032028);
        AAs.emplace_back("T",101.047679);
        AAs.emplace_back("V",99.068414);
        AAs.emplace_back("W",186.079313);
        AAs.emplace_back("Y",163.063320);
        sort();
    }

    void AATab::sort()
    {
        std::sort(AAs.begin(),AAs.end());
    }


    void AATab::__print__(std::ostream& stream)const
    {
        const unsigned int size = AAs.size();
        for(int i=0;i<size;++i)
            AAs[i].__print__(stream);
    }
}
