#include <harpe-algo/AA.hpp>

namespace harpe
{
    AA::AA(int p,const std::string& sl,double mas): pk(p),slug(sl), mass(mas)
    {
    }

    bool AA::operator<(const AA& other)const
    {
        return mass<other.mass;
    }

    void AA::__print__(std::ostream& stream)const
    {
        stream<<slug;
    }
}
