#include <harpe-algo/AATab.hpp>

#include <algorithm>

namespace harpe
{
    
    AATab::AATab()
    {
    }

    void AATab::sort()
    {
        std::sort(AAs.begin(),AAs.end());
        if(AAs.size()>0)
        {
            min_mass = AAs.front().getMass();
            max_mass = AAs.back().getMass();
        }
    }


    void AATab::__print__(std::ostream& stream)const
    {
        const unsigned int size = AAs.size();
        for(int i=0;i<size;++i)
            AAs[i].__print__(stream);
    }
}
