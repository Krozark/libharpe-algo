#ifndef HARPE_AATAB_HPP
#define HARPE_AATAB_HPP

#include <harpe-algo/AA.hpp>
#include <string>


#include <vector>
#include <iostream>

namespace harpe
{
    ///\todo AATab
    class AATab
    {
        public:
            AATab();
            AATab(const AATab&) = delete;
            AATab& operator=(const AATab&) = delete;


            void sort();

            inline const harpe::AA& operator[](const int i){return AAs[i];}

            inline unsigned int size()const{return AAs.size();}

            inline double getMax()const{return max_mass;}
            inline double getMin()const{return min_mass;}

            void __print__(std::ostream& stream)const;

            template <typename ... Args>
            inline void add(Args ... args){AAs.emplace_back(args...);}

        protected:

        private:
            std::vector<harpe::AA> AAs;

            double max_mass,
                   min_mass;
    };
}
#endif
