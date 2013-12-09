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
            static std::list<harpe::Sequence> analyse(const mgf::Spectrum& s,int debut=-1);

            typedef std::list<harpe::SequenceToken*> pile_tokens_ptr;

        protected:
            enum Sens{LEFT=-1,STOP=0,RIGHT=1};

            static const std::vector<int> get_index_max_intensitee_vector(const mgf::Spectrum& spectrum,const int nb);
            static std::vector<SequenceToken*> get_near(const std::vector<mgf::Peak*>& peak_list,const int index, const Sens inc,std::vector<SequenceToken*> tokens_ptr);

            static int pop_stack(pile_tokens_ptr& search,const int sens);

            static void save_stack(const pile_tokens_ptr& search,const mgf::Spectrum& spectrum,std::list<Sequence>& res);

            /**
             * \todo set header
             */
            static void merge_solution(std::list<Sequence>& finds,const std::list<Sequence>& left_part,const std::list<Sequence>& right_part); //met tout dasn left
            



            static void __print__(const std::vector<SequenceToken*>&,std::ostream& stream);
            static void __print__(const std::list<Sequence>&,std::ostream& stream);
            
            
        private:
            mgf::Driver driver;
    };
}
#endif
