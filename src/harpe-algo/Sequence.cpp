#include <harpe-algo/Sequence.hpp>
#include <harpe-algo/Context.hpp>
#include <harpe-algo/defines.hpp>

#include <mgf/Spectrum.hpp>
#include <mgf/Convert.hpp>

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
                cout<<MGF_BLEU<<"<"<<pep->special_peaks[Parser::peptide::DEBUT]->masse<<">"<<BLANC;
            else
            {
                pr == true;
                cout<<(sens==Sens::RIGHT?MAGENTA:ROUGE)<<"<"<<v_1.peak_token.pt_data->masse<<">"<<BLANC;
            }

            if(v_1.peak_token.pt_data != pep->special_peaks[Parser::peptide::DEBUT_H2O])
                cout<<BLEU<<"-[H2O]<"<<pep->special_peaks[Parser::peptide::DEBUT_H2O]->masse<<">"<<BLANC;
            else
            {
                pr == true;
                cout<<(sens==Sens::RIGHT?MAGENTA:ROUGE)<<"-<"<<v_1.peak_token.pt_data->masse<<">"<<BLANC;
            }

            if (not pr)
                cout<<VERT<<"-?-"<<(sens==Sens::RIGHT?MAGENTA:ROUGE)<<"<"<<v_1.peak_token.pt_data->masse<<">"<<BLANC;
        }*/

        for(int i=0;i<size;++i)
        {
            const SequenceToken& v_i = *sequence[i];

            if(v_i.type == SequenceToken::AA_TOKEN)
            {
                stream<<MGF_JAUNE<<"-"<<Context::aa_tab[v_i.aa_token.index].getSlug();
            }
            /*else if (i < size-1 and v_i.type == SequenceToken::PEAK_TOKEN)
                stream<<MGF_VERT<<"-<"<<v_i.peak_token.pt_data->getMasse()<<">";*/
        }
        stream<<MGF_BLANC;

        /*if (p)
        {
            bool pr = false;
            const stack_token& v_size_1 = *v[size-1];

            if(v_size_1.peak_token.pt_data != pep->special_peaks[Parser::peptide::FIN_H2O] and v_size_1.peak_token.pt_data != pep->special_peaks[Parser::peptide::FIN])
                cout<<(sens==Sens::LEFT?MAGENTA:ROUGE)<<"-<"<<v_size_1.peak_token.pt_data->masse<<">-"<<VERT<<"?"<<BLANC;
            else
            {
                if(v_size_1.peak_token.pt_data == pep->special_peaks[Parser::peptide::FIN_H2O])
                {
                    pr = true;
                    cout<<(sens==Sens::RIGHT?ROUGE:BLEU)<<"-<"<<pep->special_peaks[Parser::peptide::FIN_H2O]->masse<<">[H2O]"<<BLANC;
                }

                if(not pr and v_size_1.peak_token.pt_data == pep->special_peaks[Parser::peptide::FIN])
                {
                    pr = true;
                    cout<<(sens==Sens::RIGHT?ROUGE:BLEU)<<"-<"<<pep->special_peaks[Parser::peptide::FIN]->masse<<">"<<BLANC;
                }
            }

            if(not pr and v_size_1.peak_token.pt_data != pep->special_peaks[Parser::peptide::FIN_H2O])
                cout<<BLEU<<"-<"<<pep->special_peaks[Parser::peptide::FIN_H2O]->masse<<">[H2O]"<<BLANC;

            if(not pr and v_size_1.peak_token.pt_data != pep->special_peaks[Parser::peptide::FIN])
                cout<<BLEU<<"-<"<<pep->special_peaks[Parser::peptide::FIN]->masse<<">"<<BLANC;
        }*/
    };

    void Sequence::initHeader(const mgf::Spectrum& spectrum)
    {
        double values[Stats::SIZE] = {0.f};

        values[Stats::MASSE_PARENT]=spectrum.getMasse();
        values[Stats::INTENSITEE_TOTAL_PARENT] += spectrum.getMeta().intensity_sum;

        auto i=sequence.begin();
        auto end = sequence.end();

        while(i!=end)
        {
            const SequenceToken& ii = **i;

            if (ii.type == SequenceToken::Type::AA_TOKEN)
            {
                ++values[Stats::NB_AA];
                values[Stats::MASSE_TOTAL] += Context::aa_tab[ii.aa_token.index].getMasse();

                values[Stats::ERROR_AA_CUMUL] += ABS(ii.aa_token.error);
                values[Stats::ERROR_TOTAL] += ii.aa_token.error;
            }
            if (ii.type == SequenceToken::Type::PEAK_TOKEN)
            {
                ++values[Stats::NB_PEAKS];
                mgf::Peak* p = ii.peak_token.pt_data;
                values[Stats::INTENSITEE_TOTAL] += p->getIntensity();
                if ( spectrum.is_one_of_h2o(p))
                    values[Stats::MASSE_TOTAL] += mgf::Convert::MH2O;
            }
            ++i;
        }

        values[Stats::PERCENT_COUVERTURE] = values[Stats::MASSE_TOTAL] / values[Stats::MASSE_PARENT];
        values[Stats::PERCENT_INTENSITEE_UTILISEE] = values[Stats::INTENSITEE_TOTAL] /100.0 * values[Stats::INTENSITEE_TOTAL_PARENT];

        header.score = Context::calc_score(values);

    }
}
