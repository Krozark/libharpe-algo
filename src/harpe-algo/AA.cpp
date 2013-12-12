#include <harpe-algo/AA.hpp>

namespace harpe
{
    AA::AA(int p,const std::string& sl,double mas): pk(p),slug(sl), masse(mas)
    {
    }

    bool AA::operator<(const AA& other)const
    {
        return masse<other.masse;
    }

    void AA::__print__(std::ostream& stream)const
    {
        stream<<slug;
    }
}
