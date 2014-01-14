#ifndef HARPE_DB_AA_HPP
#define HARPE_DB_AA_HPP

#include <string>

namespace harpe
{
    /**
     * \brief A class that replesent a amino acide
     */
    class AA
    {
        public:
            AA(const AA&) = default;
            AA& operator=(const AA&) = default;

            /**
             * \brief constuctor
             * \param p the pk of the AA (id in the data bas, for store results)
             * \param sl the slug for the debug print
             * \param mas the mass the the AA
             */
            AA(int p,const std::string& sl,double mas);

            /**
             * \brief compare 2 AA by ther mass
             */
            bool operator<(const AA& other)const;

            /**
             * \brief debug print
             */
            void __print__(std::ostream& stream)const;

            /**
             * \return the mass of the AA
             */
            inline double getMass()const{return mass;}

            /**
             * \return the slug
             */
            inline const std::string& getSlug()const{return slug;}

            /**
             * \return the pk
             */
            inline int getPk()const{return pk;};

        private:
            int pk;///< the pk in bdd
            std::string slug; ///< the slug (unique)
            double mass; ///< the mass
    };
}
#endif
