#ifndef HARPE_SEQUENCETOKEN_HPP
#define HARPE_SEQUENCETOKEN_HPP

#include <vector>

#include <mgf/Peak.hpp>


namespace harpe
{
    class Analyser;
    class SequenceToken;
    /**
     * \brief A class to manage sequence tokens. They can be use during the analyse of the Spectrum
     */
    class SequenceToken
    {
        public:

            /**
             * \brief Constructor for containers
             */
            SequenceToken();
            
           /**
            * \brief Copy constructor
            */ 
            SequenceToken(const SequenceToken&);

            /**
             * \brief Same as copy constructor
             */
            SequenceToken& operator=(const SequenceToken&);

            /**
             * \brief Move constructor
             */
            SequenceToken(SequenceToken&& tmp);

            /**
             * \brief Construct a PEAK_TOKEN
             *
             * \param i index of peak
             * \param peak pointer
             **/
            SequenceToken(const int index,mgf::Peak* peak);

            /**
             * \brief Construct a AA_TOKEN
             *
             * \param i index of tha amino acide in AminoAcideManager
             * \param e error between masses
             * \param peak Peak where the AA is find
             **/
            SequenceToken(const int i,const double e,SequenceToken* peak); //aa
    
            /**
             * \brief Destructor
             */
            ~SequenceToken();

            /**
             * \brief Debug print. Print all the data (type, and related)
             */
            void __print__(std::ostream& stream)const;

            /**
             * \brief Possible type for token. Default is UNKNOW
             */
            enum Type {UNKNOW=-1, ///< Inconnus / ERREUR
                AA_TOKEN=0, ///< acide aminé
                //AA_HOLD_TOKEN, ///< acide aminé théorique
                PEAK_TOKEN ///< Pic
            };

        private:
            friend class Analyser;
            friend class Sequence;

            Type type;///< type of the current token

            /**
             * Store datas for specifique types
             */
            union {
                struct  {
                    int index;
                    double error;
                    SequenceToken* pt_data;
                } aa_token; ///< AA_TOKEN;

                /*struct{
                    double error;
                    std::vector<int> aa_index;
                } aa_hold_token; ///<AA_HOLD_TOKEN;
                */

                struct {
                    int index;
                    mgf::Peak* pt_data;
                } peak_token; ///< PEAK_TOKEN;

            };

            /**
             * \brief short cut function to get and set null ptr_data
             * \return the SequenceToken stored by the token
             */
            inline SequenceToken* get_peak_stack_NULL() {
                SequenceToken* res = aa_token.pt_data;
                aa_token.pt_data=NULL;
                return res;
            };

    };
}
#endif
