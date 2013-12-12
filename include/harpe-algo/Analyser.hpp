#ifndef HARPE_ANALYSER_HPP
#define HARPE_ANALYSER_HPP

#include <mgf/Driver.hpp>
#include <mgf/Spectrum.hpp>

#include <harpe-algo/Sequence.hpp>

#include <iostream>
#include <list>
#include <vector>

namespace harpe
{
    class Analyser
    {
        public:
            Analyser(const Analyser&) = delete;
            Analyser& operator=(const Analyser&) = delete;

            /**
             * \brief Construct a Analyser from stream
             * \param input MGF input stream
             */
            Analyser(std::istream& input);
            //Analyser(ntw::SocketSerialied& sock);
            
            /**
             * \brief analyse all the MS/MS spectrum in the parsed input, and try to build the Sequence
             */
            int analyse();

            /**
             * \brief analyse all the MS/MS spectrum in the parsed input, and try to build the Sequence
             */
            static std::vector<harpe::Sequence> analyse(const mgf::Spectrum& s,int debut=-1);

            typedef std::list<harpe::SequenceToken*> pile_tokens_ptr;

            

            static void free();
            
        private:
            static std::vector<SequenceToken*> tokens_ptr;///< stocke les stack token pour les delete à la fin de l'analyse

            enum Sens{LEFT=-1,STOP=0,RIGHT=1};

            static const std::vector<int> get_index_max_intensitee_vector(const mgf::Spectrum& spectrum,const int nb);
            static std::vector<SequenceToken*> get_near(const std::vector<mgf::Peak*>& peak_list,const int index, const Sens inc);

            static int pop_stack(pile_tokens_ptr& search,const int sens);

            static void save_stack(const pile_tokens_ptr& search,const mgf::Spectrum& spectrum,std::list<Sequence>& res);

            static void merge_solution(std::vector<Sequence>& finds,const std::list<Sequence>& left_part,const std::list<Sequence>& right_part,const mgf::Spectrum&); //met tout dasn left



            static void __print__(const std::vector<SequenceToken*>&,std::ostream& stream);
            static void __print__(const std::list<Sequence>&,std::ostream& stream);
            static void __print__(const std::vector<Sequence>&,std::ostream& stream);
            
            
        private:
            mgf::Driver driver;
    };
}
#endif
