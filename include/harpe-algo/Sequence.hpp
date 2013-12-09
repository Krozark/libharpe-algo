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
