#include <harpe-algo/Analyser.hpp>

#include <array>
#include <algorithm>
#include <iostream>

#include <harpe-algo/defines.hpp>
#include <mgf/Analyse.hpp>


namespace harpe
{
    
    Analyser::Analyser(std::istream& input) : driver(input)
    {
    }

    int Analyser::analyse()
    {
        mgf::Analyse ana = this->driver.parse();
        if (this->driver.isValid())
        {
            auto spectrums(ana.getSpectrums());
            for (mgf::Spectrum* spectrum : spectrums)
            {
                this->analyse(*spectrum);
            }
        }
    }

    std::list<harpe::Sequence> Analyser::analyse(const mgf::Spectrum& spectrum,int debut)
    {
        const std::vector<int> peaks_index = debut>0?std::vector<int>(debut):get_index_max_intensitee_vector(spectrum,HARPE_ALGO_DEFAULT_START_PEAKS_NB);

        const unsigned int index_size=peaks_index.size();
        const std::vector<mgf::Peak*>& peaks = spectrum.getPeaks();

        std::vector<SequenceToken*> tokens_ptr;///< stocke les stack token pour les delete à la fin de l'analyse
        for(unsigned int index=0;index<index_size;++index)
        {
            register int current_peak_index = peaks_index[index];

            pile_tokens_ptr search;
            std::list<Sequence> results_right;
            std::list<Sequence> results_left;

            Sens sens= Sens::RIGHT;

            tokens_ptr.emplace_back(new SequenceToken(current_peak_index,peaks[current_peak_index]));
            search.emplace_back(tokens_ptr.back());

            while (sens)
            {
                mgf::Peak* current_peak = peaks[current_peak_index];
                current_peak->setUsed(true);
            }

        }

        std::list<harpe::Sequence> res;
        return res;
    }

    const std::vector<int> Analyser::get_index_max_intensitee_vector(const mgf::Spectrum& spectrum,const int nb)
    {
        std::vector<int> res;
        std::vector<mgf::Peak*> res_peaks;
        int size_nb;
        {
            std::vector<mgf::Peak*> peaks(spectrum.getPeaks());

            size_nb = peaks.size() * spectrum.getHeader().getCharge();
            if (size_nb > peaks.size())
                size_nb = peaks.size()-1;

            partial_sort(peaks.begin(),peaks.begin()+size_nb,peaks.end(),
                         [](const mgf::Peak* _1,const mgf::Peak* _2){
                         if(_1->getCharge() >0)//real peak
                         {
                             if(_2->getCharge()>0)//real peak
                                 return _1->getIntensity() > _2->getIntensity();//real both
                             return true;//_2 is artificial
                         }
                             if (_2->getCharge()>0) //_1 is artificial
                                 return false;//_1 < _2
                         return false;//both are artificials
                         });

            res_peaks.reserve(size_nb);
            res.reserve(size_nb);

            for (int i=0;i<size_nb;++i)
                res_peaks.push_back(peaks[i]);

        }
        const std::vector<mgf::Peak*>& peaks = spectrum.getPeaks();
        const unsigned int size = peaks.size();
        int finds=0;
        for (int i=0;i<size_nb;++i)//pour tous les peaks à trouver
        {
            for(int j=0;j<size;++j)
            {
                if(res_peaks[i] == peaks[j])
                {
                    res.push_back(j);
                    if(peaks[j]->isOriginal())
                        if(++finds >=nb)
                            goto end_loop;
                    break;
                }
            }
        }
        end_loop:
        res.shrink_to_fit();
        return res;
    }
    std::vector<SequenceToken> Analyser::get_near(const std::vector<mgf::Peak*>& peak_list,const int index, const Sens inc)
    {
        std::vector<SequenceToken> res;

    }
}
