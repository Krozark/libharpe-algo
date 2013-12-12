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

        private:
            friend class Analyser;

            std::vector<SequenceToken*> sequence;///< the sequence

            /**
             * \brief store the meta datas
             */
            struct Header {
                double score;
                /*struct _holds {
                    std::shared_ptr<std::list<list_arg> > link;
                    double to_find;
                } holds [Parser::peptide::SIZE];*/
            } header;

            /**
             * \brief Enum for stats of a Sequence
             */
            enum Stats {
                ERROR_TOTAL=0, ///< les erreur + et - se compensent
                ERROR_AA_CUMUL, ///< les erreur + et - se compensnt pas
                INTENSITEE_TOTAL_PARENT,///< intensitée total des intensitées
                INTENSITEE_TOTAL, ///< intensitée totale qui est utilisée
                MASSE_TOTAL,///< somme des masse des AA
                PERCENT_COUVERTURE,///< MASSE_TOTAL /100 * MASSE_PARENT 
                //PERCENT_COUVERTURE_AA,///< MASSE_TOTAL / masse moyenne AA
                NB_AA, ///< nombre de AA dans la chaine
                NB_PEAKS, ///< nombre de peaks dasn la chaine
                MASSE_PARENT, ///< masse du peptide
                PERCENT_INTENSITEE_UTILISEE,///< INTENSITEE_TOTAL /100 * INTENSITEE_TOTAL_PARENT  
                SIZE ///< Nombre de valeurs
            };            
            
            /**
             * \brief inialyse the header of a Sequence (stats valus)
             * \param spectrum the related spectrum
             */
            void initHeader(const mgf::Spectrum& spectrum);
    };
}
#endif
