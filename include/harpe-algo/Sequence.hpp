#ifndef HARPE_SEQUENCE_HPP
#define HARPE_SEQUENCE_HPP

#include <vector>
#include <harpe-algo/SequenceToken.hpp>

namespace harpe
{
    class Sequence
    {
        public:
            Sequence(const Sequence&) = delete;
            Sequence& operator=(const Sequence&) = delete;

            Sequence();

        protected:

        private:
            std::vector<SequenceToken> sequence;

            struct Header {
                double score;
            } header;

    };
}
#endif
