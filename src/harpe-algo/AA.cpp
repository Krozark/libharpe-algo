#include <harpe-algo/AA.hpp>

#include <iostream>

#include <Socket/Serializer.hpp>

namespace harpe
{
    AA::AA(int p,const std::string& sl,double mas): 
        pk(p),
        slug(sl), 
        mass(mas),
        mod_pk(-1),
        position(POSITION::EVERY_WHERE)
    {
    }

    AA::AA(int p,const std::string& sl,double mas, int mod_p, float delta,int pos): 
        pk(p),
        slug(sl),
        mass(mas + delta),
        mod_pk(mod_p),
        position(pos)
    {
    }

    bool AA::operator<(const AA& other)const
    {
        return mass<other.mass;
    }

    void AA::__print__(std::ostream& stream)const
    {
        if(mod_pk>0)
        {
            stream << "modified(" << slug;
            switch(position)
            {
                //case 1:break; //<partout
                case 2:
                    stream<<"@N-term";
                    break;
                case 3:
                    stream<<"@C-term";
                    break;
                default:
                    break;
            }
            stream<<","<<mod_pk<<")";
        }
        else
            stream<<slug;
    }

    ntw::Serializer& operator<<(ntw::Serializer& stream,const AA& self)
    {
        stream<<self.pk<<self.mod_pk;
        return stream;
    }
}

