#ifndef HARPE_AATAB_HPP
#define HARPE_AATAB_HPP

#include <harpe-algo/AA.hpp>
#include <string>


#include <vector>
#include <iostream>

namespace harpe
{
    /**
     * \brief A class to store AA tha can bve used
     * during the research
     */
    class AATab
    {
        public:
            AATab(const AATab&) = delete;
            AATab& operator=(const AATab&) = delete;

            /**
             * \brief Default constructor
             */
            AATab();

            /**
             * \brief Sort the AA stored by ther mass.
             * This function have to be call before any run, for iternal optimisations.
             * This also set the min and max mass values
             */
            void sort();

            /**
             * \return A AA by his index
             */
            inline const harpe::AA& operator[](const int i){return AAs[i];}

            /**
             * \return the number of AAs
             */
            inline unsigned int size()const{return AAs.size();}

            /**
             * \return the max mass of aa (for internal optimisation)
             */
            inline double getMax()const{return max_mass;}

            /**
             * \return the mass of the min mass of aa (for internal optimisation)
             */
            inline double getMin()const{return min_mass;}

            /**
             * \brief debug print of all AA
             */
            void __print__(std::ostream& stream)const;

            /**
             * \brief add a AA to the inten contener.
             * \param args The AA constructor arguments
             */
            template <typename ... Args>
            inline void add(Args ... args){AAs.emplace_back(args...);}

            inline void clear(){AAs.clear();max_mass=min_mass=0;}

        protected:

        private:
            std::vector<harpe::AA> AAs; ///< store the AAs

            double max_mass, ///< maximal mass of the AA.
                   min_mass;///< minimal mass of the AA
    };
}
#endif
