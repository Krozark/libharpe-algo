#ifndef HARPE_AATAB_HPP
#define HARPE_AATAB_HPP

#include <string>

namespace harpe
{
    namespace db
    {
        ///\todo AA
        class AA ///\todo Make it
        {
            public:
                AA(const std::string& sl,double mas): slug(sl), masse(mas){}

                bool operator<(const AA& other)const{return masse<other.masse;}

                inline const std::string& getSlug()const{return slug;}

                void __print__(std::ostream& stream)const{}

                inline double getMasse()const{return masse;}

            private:
                std::string slug;
                double masse;
        };
    }
}

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

            inline const harpe::db::AA& operator[](const int i){return AAs[i];}

            inline unsigned int size()const{return AAs.size();}

            inline double getMax()const{return max_mass;}
            inline double getMin()const{return min_mass;}

            void __print__(std::ostream& stream)const;

        protected:

        private:
            std::vector<harpe::db::AA> AAs;

            double max_mass,
                   min_mass;
    };
}
#endif
