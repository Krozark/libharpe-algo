#ifndef HARPE_SEQUENCE_HPP
#define HARPE_SEQUENCE_HPP

#include <vector>
#include <harpe-algo/SequenceToken.hpp>

namespace harpe
{
    class Analyser;
    class Sequence
    {
        public:
            Sequence(const Sequence&) = default;
            Sequence& operator=(const Sequence&) = default;

            Sequence();
            
            /**
             * \brief Debug print. Print all the data (type, and related)
             */
            void __print__(std::ostream& stream)const;

            /**
             * \brief Permet d'afficher la séquence en AA de la proposition
             *
             * \param v la séquence
             * \param sens Sens de lecture
             * \param p affiche les "?" et les masse
             **/
            void __print_AA__(std::ostream& stream)const;

        protected:

        private:
            friend class Analyser;

            std::vector<SequenceToken*> sequence;

            struct Header {
                double score;
                /*struct _holds {
                    std::shared_ptr<std::list<list_arg> > link;
                    double to_find;
                } holds [Parser::peptide::SIZE];*/
            } header;

    };
}
#endif
