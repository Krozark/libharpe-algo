#include <harpe-algo/Sequence.hpp>
#include <harpe-algo/Context.hpp>
#include <harpe-algo/defines.hpp>

#include <mgf/Spectrum.hpp>
#include <mgf/Convert.hpp>

#include <Socket/Serializer.hpp>

namespace harpe
{
    
    Sequence::Sequence()
    {
    }

    void Sequence::__print__(std::ostream& stream)const
    {
        for(SequenceToken* tok : sequence)
            tok->__print__(stream);
    }

    void Sequence::__print_AA__(std::ostream& stream)const
    {
        int size = sequence.size();
        stream<<"score: ["<<header.score<<"] ";

        /*if (p)
        {
            bool pr=false;
            const SequenceToken& v_1 = *Sequence[1];

            if(v_1.peak_token.pt_data != pep->special_peaks[Parser::peptide::DEBUT])
                cout<<MGF_BLEU<<"<"<<pep->special_peaks[Parser::peptide::DEBUT]->mass<<">"<<BLANC;
            else
            {
                pr == true;
                cout<<(sens==Sens::RIGHT?MAGENTA:ROUGE)<<"<"<<v_1.peak_token.pt_data->mass<<">"<<BLANC;
            }

            if(v_1.peak_token.pt_data != pep->special_peaks[Parser::peptide::DEBUT_H2O])
                cout<<BLEU<<"-[H2O]<"<<pep->special_peaks[Parser::peptide::DEBUT_H2O]->mass<<">"<<BLANC;
            else
            {
                pr == true;
                cout<<(sens==Sens::RIGHT?MAGENTA:ROUGE)<<"-<"<<v_1.peak_token.pt_data->mass<<">"<<BLANC;
            }

            if (not pr)
                cout<<VERT<<"-?-"<<(sens==Sens::RIGHT?MAGENTA:ROUGE)<<"<"<<v_1.peak_token.pt_data->mass<<">"<<BLANC;
        }*/

        for(int i=0;i<size;++i)
        {
            const SequenceToken& v_i = *sequence[i];

            if(v_i.type == SequenceToken::AA_TOKEN)
            {
                stream<<MGF_JAUNE<<"-"<<Context::aa_tab[v_i.aa_token.index].getSlug();
            }
            else if (/*i < size-1 and */v_i.type == SequenceToken::PEAK_TOKEN)
                stream<<MGF_VERT<<"-<"<<v_i.peak_token.pt_data->getMass()<<">";
        }
        stream<<MGF_BLANC;

        /*if (p)
        {
            bool pr = false;
            const stack_token& v_size_1 = *v[size-1];

            if(v_size_1.peak_token.pt_data != pep->special_peaks[Parser::peptide::FIN_H2O] and v_size_1.peak_token.pt_data != pep->special_peaks[Parser::peptide::FIN])
                cout<<(sens==Sens::LEFT?MAGENTA:ROUGE)<<"-<"<<v_size_1.peak_token.pt_data->mass<<">-"<<VERT<<"?"<<BLANC;
            else
            {
                if(v_size_1.peak_token.pt_data == pep->special_peaks[Parser::peptide::FIN_H2O])
                {
                    pr = true;
                    cout<<(sens==Sens::RIGHT?ROUGE:BLEU)<<"-<"<<pep->special_peaks[Parser::peptide::FIN_H2O]->mass<<">[H2O]"<<BLANC;
                }

                if(not pr and v_size_1.peak_token.pt_data == pep->special_peaks[Parser::peptide::FIN])
                {
                    pr = true;
                    cout<<(sens==Sens::RIGHT?ROUGE:BLEU)<<"-<"<<pep->special_peaks[Parser::peptide::FIN]->mass<<">"<<BLANC;
                }
            }

            if(not pr and v_size_1.peak_token.pt_data != pep->special_peaks[Parser::peptide::FIN_H2O])
                cout<<BLEU<<"-<"<<pep->special_peaks[Parser::peptide::FIN_H2O]->mass<<">[H2O]"<<BLANC;

            if(not pr and v_size_1.peak_token.pt_data != pep->special_peaks[Parser::peptide::FIN])
                cout<<BLEU<<"-<"<<pep->special_peaks[Parser::peptide::FIN]->mass<<">"<<BLANC;
        }*/
    };

    ntw::Serializer& operator<<(ntw::Serializer& stream,const Sequence& self)
    {
        stream<<self.header.score;

        for(unsigned int i=0;i<Sequence::Stats::SIZE;++i)
            stream<<self.header.stats[i];

        stream<<(unsigned int)self.sequence.size();

        for(SequenceToken* t : self.sequence)
            stream<<*t;
        return stream;
    }

    std::string Sequence::to_string()const
    {
        std::string res;
        int size = sequence.size();

        if(size>1)
        {
            const SequenceToken& v_i = *sequence[1]; //aa
            res+=Context::aa_tab[v_i.aa_token.index].getSlug();
        }

        for(int i=3;i<size;i+=2)
        {
            const SequenceToken& v_i = *sequence[i];
            res+="-"+Context::aa_tab[v_i.aa_token.index].getSlug();
        }
        return res;
    }

    bool Sequence::isValid(const mgf::Spectrum& spectrum)const
    {
        const unsigned int size = sequence.size();
        /*bool res = true;
        if (size >= 1+2+2)//peak+(AA,peak)*2
        {
            //peak(,AA,peak)*
            const AA& aa_firt = Context::aa_tab[sequence[1]->aa_token.index];
            const AA& aa_last = Context::aa_tab[sequence[size-2]->aa_token.index];

            //if [0] and [-1] == (N_TER or C_TER)
            if(aa_firt.position == aa_last.position and (aa_firt.position != AA::POSITION::EVERY_WHERE))
                res = false;
            else
            {
                for(unsigned int i = 3; i<size-4 and res;i+=2)
                {
                    res = (Context::aa_tab[sequence[i]->aa_token.index].position == AA::POSITION::EVERY_WHERE);
                }
            }

        }*/
        if(size >= 1+2)//1 AA
        {
            const AA& first_aa = Context::aa_tab[sequence[1]->aa_token.index];
            const mgf::Peak* first_peak = sequence[0]->peak_token.pt_data; 
            bool first_aa_term = false;

            if(first_aa.position == AA::POSITION::N_TER or first_aa.position == AA::POSITION::C_TER)//first is tern
            {
                first_aa_term = true;
                if(not spectrum.is_one_of_specials(first_peak)) //not good peak
                    return false;
            }

            if (size >= 1+2+2)//2 AA
            {
                const AA& last_aa = Context::aa_tab[sequence[size-2]->aa_token.index];
                const mgf::Peak* last_peak = sequence[size-1]->peak_token.pt_data;
                bool last_aa_term = false;

                if(last_aa.position == AA::POSITION::N_TER or last_aa.position == AA::POSITION::C_TER)//AA are C or N terminal only
                {
                    last_aa_term = true;
                    if (not spectrum.is_one_of_specials(last_peak)) // not C on N ter peaks
                        return false;
                }

                if(first_aa_term and last_aa_term and first_aa.position == last_aa.position)//all are C_ter or all N_ter
                    return false;

                for(unsigned int i = 3; i<size-4;i+=2)//others AA
                {
                    if(Context::aa_tab[sequence[i]->aa_token.index].position != AA::POSITION::EVERY_WHERE) //can't be terminal
                        return false;
                }
            }
        }
        return true;
    }

    const Sequence::Header& Sequence::getHeader()const
    {
        return header;
    }

    void Sequence::initHeader(const mgf::Spectrum& spectrum)
    {
        for(unsigned int i=0;i<Stats::SIZE;++i)
            header.stats[i] = 0.f;

        header.stats[Stats::MASS_PARENT]=spectrum.getMass();
        header.stats[Stats::INTENSITEE_TOTAL_PARENT] += spectrum.getMeta().intensity_sum;


        auto i=sequence.begin();
        auto end = sequence.end();

        while(i!=end)
        {
            const SequenceToken& ii = **i;

            if (ii.type == SequenceToken::Type::AA_TOKEN)
            {
                ++header.stats[Stats::NB_AA];
                header.stats[Stats::MASS_TOTAL] += Context::aa_tab[ii.aa_token.index].getMass();
                header.stats[Stats::NB_AA_MODS] += Context::aa_tab[ii.aa_token.index].isMod();

                header.stats[Stats::ERROR_AA_CUMUL] += ABS(ii.aa_token.error);
                header.stats[Stats::ERROR_TOTAL] += ii.aa_token.error;
            }
            if (ii.type == SequenceToken::Type::PEAK_TOKEN)
            {
                //++header.stats[Stats::NB_PEAKS];
                mgf::Peak* p = ii.peak_token.pt_data;
                header.stats[Stats::INTENSITEE_TOTAL] += p->getIntensity();
                /*if ( spectrum.is_one_of_h2o(p))
                    header.stats[Stats::MASS_TOTAL] += mgf::Convert::MH2O;*/
            }
            ++i;
        }

        header.stats[Stats::PERCENT_COUVERTURE] = header.stats[Stats::MASS_TOTAL] / header.stats[Stats::MASS_PARENT];
        header.stats[Stats::PERCENT_INTENSITEE_UTILISEE] = header.stats[Stats::INTENSITEE_TOTAL] / header.stats[Stats::INTENSITEE_TOTAL_PARENT];

        header.score = Context::calc_score(header.stats);

    }
}
