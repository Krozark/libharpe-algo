#ifndef HARPE_SEQUENCE_HPP
#define HARPE_SEQUENCE_HPP

#include <vector>
#include <harpe-algo/SequenceToken.hpp>

namespace harpe
{
    class Analyser;

    /**
     * \brief A class thaty stor a single Sequence for a analyse
     */
    class Sequence
    {
        public:
            Sequence(const Sequence&) = default;
            Sequence& operator=(const Sequence&) = default;

            /**
             * \brief Constructor
             */
            Sequence();
            
            /**
             * \brief Debug print. Print all the data (type, and related)
             */
            void __print__(std::ostream& stream)const;

            /**
             * \brief debug print
             **/
            void __print_AA__(std::ostream& stream)const;

            /**
             * \brief store the Sequence
             * double score,
             * double stats[Stats::SIZE],
             * unsigned int size,
             * SequenceToken[size]
             */
            friend ntw::Serializer& operator<<(ntw::Serializer& stream,const Sequence& self);


            /**
             * \return a string as <AA slug>-<AA slug> ...
             */
            std::string to_string()const;

            /**
             * \brief Enum for stats of a Sequence
             */
            enum Stats {
                ERROR_TOTAL=0, ///< les erreur + et - se compensent
                ERROR_AA_CUMUL, ///< les erreur + et - se compensnt pas
                INTENSITEE_TOTAL_PARENT,///< intensitée total des intensitées
                INTENSITEE_TOTAL, ///< intensitée totale qui est utilisée
                MASS_TOTAL,///< somme des mass des AA
                PERCENT_COUVERTURE,///< MASS_TOTAL /100 * MASS_PARENT 
                //PERCENT_COUVERTURE_AA,///< MASS_TOTAL / mass moyenne AA
                NB_AA, ///< nombre de AA dans la chaine
                NB_PEAKS, ///< nombre de peaks dasn la chaine
                MASS_PARENT, ///< mass du peptide
                PERCENT_INTENSITEE_UTILISEE,///< INTENSITEE_TOTAL /100 * INTENSITEE_TOTAL_PARENT  
                SIZE ///< Nombre de valeurs
            };            

            /**
             * \brief store the meta datas
             */
            struct Header {
                double score; ///< store the score
                double stats[Stats::SIZE]; ///< store all the stats for score calc
                /*struct _holds {
                    std::shared_ptr<std::list<list_arg> > link;
                    double to_find;
                } holds [Parser::peptide::SIZE];*/
            };
            
            const Header& getHeader()const;

        private:
            friend class Analyser;

            std::vector<SequenceToken*> sequence;///< the sequence

            Header header;
            
            /**
             * \brief inialyse the header of a Sequence (stats valus)
             * \param spectrum the related spectrum
             */
            void initHeader(const mgf::Spectrum& spectrum);
    };
}
#endif
