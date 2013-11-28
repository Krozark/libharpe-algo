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
    }


    void AATab::__print__(std::ostream& stream)const
    {
        const unsigned int size = AAs.size();
        for(int i=0;i<size;++i)
            AAs[i].__print__(stream);
    }
}
