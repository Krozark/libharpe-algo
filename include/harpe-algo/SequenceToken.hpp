#ifndef HARPE_SEQUENCETOKEN_HPP
#define HARPE_SEQUENCETOKEN_HPP

namespace harpe
{
    class SequenceToken
    {
        public:
            SequenceToken();
            SequenceToken(const SequenceToken&) = delete;
            SequenceToken& operator=(const SequenceToken&) = delete;

        protected:

        private:
            enum Type {UNKNOW=-1, ///< Inconnus / ERREUR
                AA_Token=0, ///< acide aminé
                AA_HOLD_Token, ///< acide aminé théorique
                PEAK_Token ///< Pic
            } type;
    };
}
#endif
