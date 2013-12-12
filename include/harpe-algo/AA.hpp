#ifndef HARPE_DB_AA_HPP
#define HARPE_DB_AA_HPP

#include <string>

namespace harpe
{
    class AA
    {
        public:
            AA(const AA&) = default;
            AA& operator=(const AA&) = default;

            AA(int p,const std::string& sl,double mas);

            bool operator<(const AA& other)const;

            void __print__(std::ostream& stream)const;

            inline double getMasse()const{return masse;}
            inline const std::string& getSlug()const{return slug;}

        private:
            int pk;
            std::string slug;
            double masse;
    };
}
#endif
