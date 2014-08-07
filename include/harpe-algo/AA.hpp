#ifndef HARPE_DB_AA_HPP
#define HARPE_DB_AA_HPP

#include <string>

namespace ntw
{
    class Serializer;
}


namespace harpe
{

    class SequenceToken;

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
             * \brief constuctor
             * \param p the pk of the AA (id in the data bas, for store results)
             * \param sl the slug for the debug print
             * \param mas the mass the the AA
             * \param mod_p the modification pk
             * \param delta the mass delta of the modification
             * \param position the modification pos value. CHOICES = 1:partout,2:N-term,3:C-term"uuuuu
             */
            AA(int p,const std::string& sl,double mas,int mod_p,float delta, int pos);

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
             * \brief output the AA
             */            
            friend ntw::Serializer& operator<<(ntw::Serializer& stream,const AA& self);

            bool isMod()const{return mod_pk>0;}



        private:
            friend class SequenceToken;
            friend class Sequence;

            int pk;///< the AA pk in bdd
            std::string slug; ///< the slug (unique)
            double mass; ///< the mass
            
            int mod_pk;//< modification pk in bdd
            int position; ///< CHOICES = ((1,"partout"),(2,"N-term"),(3,"C-term")

            /**
             * \brief a enum for the position value
             */
            enum POSITION {
                EVERY_WHERE = 1,
                N_TER = 2,
                C_TER = 3
            };
    };
}
#endif
