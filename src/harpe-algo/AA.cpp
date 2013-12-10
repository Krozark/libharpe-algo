#include <harpe-algo/AA.hpp>

namespace harpe
{
    namespace db
    {
        
        AA::AA(const std::string& sl,double mas): slug(sl), masse(mas)
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
}
