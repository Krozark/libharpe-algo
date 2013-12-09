#include <harpe-algo/Analyser.hpp>

#include <array>
#include <algorithm>
#include <iostream>

#include <harpe-algo/defines.hpp>
#include <harpe-algo/Context.hpp>
#include <mgf/Analyse.hpp>

#define eq_error(value,to_find,error) (value >= (to_find - error) && value <= (to_find + error))

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
        std::list<Sequence> finds;// results

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

                std::vector<SequenceToken*> near=get_near(peaks,current_peak_index,sens,tokens_ptr);
                const int size_near = near.size();
                if(size_near<=0)
                {
                    current_peak_index = pop_stack(search,sens);

                    if(current_peak_index == -1)
                    {
                        switch(sens)
                        {
                            case Sens::RIGHT : 
                            {
                                sens = Sens::LEFT;
                                current_peak_index = search.front()->peak_token.index;
                            }break;
                            case Sens::LEFT :
                            {
                                sens = Sens::STOP;
                            }break;
                            default:
                                HARPE_ALGO_ERROR("Unknow sens variable value")
                            break;
                            
                        }
                    }
                }
                else
                {
                    switch(sens)
                    {
                        case Sens::RIGHT : 
                        {
                            for (int i=0;i<size_near-1;++i)
                            {
                                search.emplace_back(near[i]);
                            }

                            auto& tmp_find_last = *near[size_near-1];
                            SequenceToken* current_stack_peak = tmp_find_last.get_peak_stack_NULL();

                            current_peak_index = (current_stack_peak->peak_token.index);
                            search.emplace_back(&tmp_find_last); //AA
                            search.emplace_back(current_stack_peak); //PEAK

                            save_stack(search,spectrum,results_right);
                        }break;
                        case Sens::LEFT :
                        {
                            auto& tmp_find_last = *near[size_near-1];

                            SequenceToken* current_stack_peak = tmp_find_last.get_peak_stack_NULL();

                            current_peak_index = (current_stack_peak->peak_token.index);
                            search.emplace_front(&tmp_find_last); //AA

                            for (int i=size_near-2;i>=0;--i)
                            {
                                search.emplace_front(near[i]);
                            }

                            search.emplace_front(current_stack_peak); //PEAK

                            save_stack(search,spectrum,results_left);
                        }break;
                        default:
                            HARPE_ALGO_ERROR("Unknow sens variable value")
                                break;

                    }
                }
            }

            std::cout<<"-- RIGHT --"<<std::endl;
            __print__(results_right,std::cout);
            std::cout<<"-- LEFT --"<<std::endl;
            __print__(results_left,std::cout);

            merge_solution(finds,results_left,results_right);
        }

        std::cout<<"-- FINDS --"<<std::endl;
        __print__(finds,std::cout);

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

    std::vector<SequenceToken*> Analyser::get_near(const std::vector<mgf::Peak*>& peak_list,const int index, const Sens sens,std::vector<SequenceToken*> tokens_ptr)
    {
        ///\todo
        std::vector<SequenceToken*> res;

        const unsigned int size_pep = peak_list.size();
        const unsigned int size_aa = Context::aa_tab.size();
        const double initial_masse = peak_list[index]->getMasse();
        const static double max_masse = Context::aa_tab.getMax();


        if (sens >= 0) //++i Sens::RIGHT
        {
            for (unsigned int i=index+1;i<size_pep;++i) // loop peaks
            {
                if (peak_list[i]->isUsed() /*or not pep->peaks[i]->bruit*/) // si il est déja pris
                    continue;

                const double current_masse = peak_list[i]->getMasse();
                if (initial_masse + max_masse + Context::error < current_masse) // si la masse est <= que la masse du plus gros AA (on cherche ici que les peak corespondant à 1 AA)
                    break;

                for(unsigned register int j=0;j<size_aa;++j) //on cherche l'AA qui corespond à cette différence de masse
                {
                    const double aa_masse = initial_masse + Context::aa_tab[j].getMasse();
                    if(eq_error(current_masse,aa_masse,Context::error)) // avec une marge d'erreur
                    {
                        SequenceToken* tmp = new SequenceToken(i,peak_list[i]);
                        tokens_ptr.emplace_back(tmp);

                        tmp = new SequenceToken(j,current_masse - aa_masse,tmp);
                        tokens_ptr.emplace_back(tmp);
                        res.emplace_back(tokens_ptr.back());
                    }
                }
            }
        }
        else //--i Sens::LEFT
        {
            for (int i=index-1;i>=0;--i) // loop peaks
            {
                if (peak_list[i]->isUsed() /*or not pep->peaks[i]->bruit*/) // si il est déja pris
                    continue;

                const double current_masse = peak_list[i]->getMasse();
                if (initial_masse - max_masse - Context::error > current_masse) // si la masse est >= que la masse du plus gros AA (on cherche ici que les peak corespondant à 1 AA)
                    break;

                for(unsigned int j=0;j<size_aa;++j) //on cherche l'AA qui corespond à cette différence de masse
                {
                    const double aa_masse = initial_masse - Context::aa_tab[j].getMasse();
                    if(eq_error(current_masse,aa_masse,Context::error)) // avec une marge d'erreur
                    {
                        tokens_ptr.emplace_back(new SequenceToken(i,peak_list[i]));
                        tokens_ptr.emplace_back(new SequenceToken(j,current_masse - aa_masse,tokens_ptr.back()));
                        res.emplace_back(tokens_ptr.back());
                    }
                }
            }
        }
            
        return res;
    }

    int Analyser::pop_stack(Analyser::pile_tokens_ptr& search,const int sens)
    {
        int current_peak_index = -1;

        register int nb;
        SequenceToken* token;

        if (sens == Sens::RIGHT) // depiler depuis le back
        {
            Analyser::pile_tokens_ptr::reverse_iterator i;

remove_1_peak_right:

            nb=0;
            i= search.rbegin();
            ++i;
            {
                auto end = search.rend();
                while(i!=end && (*i)->type != SequenceToken::Type::PEAK_TOKEN && nb < 2)
                {
                    ++i;
                    ++nb;
                }
            }
            if (nb==0)
            {//la pile est vide
                current_peak_index = -1;
                goto end;
            }
            token = search.back();

            search.pop_back(); // delete Peak
            token->peak_token.pt_data->setUsed(false); //on ne l'utilise plus

            search.pop_back(); // delete AA

            if (nb ==1)
            {
                nb=0;
                goto remove_1_peak_right;
            }
            else
            {
                token = search.back()->get_peak_stack_NULL();
                search.emplace_back(token);
                current_peak_index = token->peak_token.index;
            }
        }
        else if (sens == Sens::LEFT) // depiler depuis le fron
        {
            Analyser::pile_tokens_ptr::iterator i;

remove_1_peak_left:

            nb=0;
            token = search.front();
            token->peak_token.pt_data->setUsed(false); //on ne l'utilise plus

            search.pop_front(); // delete Peak

            /// trouver le prochain peak
            i= search.begin();
            {
                auto end = search.end();
                while(i!= end && (*i)->type != SequenceToken::Type::PEAK_TOKEN)
                {
                    ++i;
                    ++nb;
                }
            }

            if (nb==0)
            {//la pile est vide
                current_peak_index = -1;
                goto end;
            }

            //revenir au AA courament utilisé
            --i;

            i=search.erase(i);//delete le AA qui était utilisé

            //il n'y avait que le AA qui était pris en compt
            //on le suprime
            if (nb==1)
            {
                nb=0;
                //on recommence
                goto remove_1_peak_left;
            }
            // si il y avait d'autres solutions
            else
            {
                //on prend le AA juste avant
                --i;
                //on récupère son peak
                token = (*i)->get_peak_stack_NULL();
                //on l'ajoute
                search.push_front(token);
                current_peak_index = token->peak_token.index;
            }
        }
    end: 
        return current_peak_index;
    }


    void Analyser::save_stack(const pile_tokens_ptr& search,const mgf::Spectrum& spectrum,std::list<Sequence>& res)
    {
        Sequence sequence;
        auto i=search.begin();
        auto end = search.end();

        /*int nb=0;
        double masse = 0;
        double intensitee = spectrum.getHeader().getIntensity();
        double errors = 0;
        double error_tot = 0;
        */

        while(i!=end)
        {
            SequenceToken& tmp_i= **i;

            if (tmp_i.type == SequenceToken::Type::PEAK_TOKEN)
            {
                sequence.sequence.emplace_back(&tmp_i);
                /*Parser::peptide::peak* p = tmp_i.peak_token.pt_data;
                intensitee += p->intensitee;
                if ( pep->is_one_of_h2o(p))
                    masse += MH2O;
                */
            }
            else if (tmp_i.type == SequenceToken::Type::AA_TOKEN and tmp_i.aa_token.pt_data==NULL)
            {
                sequence.sequence.emplace_back(&tmp_i);
                /*errors += ABS(tmp_i.aa_token.error);
                error_tot += tmp_i.aa_token.error;
                masse += aa_tab[tmp_i.aa_token.index].masse;
                ++nb;
                */
            }
            ++i;
        }
        sequence.sequence.shrink_to_fit();
        //add the current
        res.emplace_back(sequence);
        //add all other possibilites tha can be (or not) complete
        --i;//peaks
        --i;//current head
        end = search.begin();//decement
        const int size =sequence.sequence.size();
        while(i!=end)
        {
            SequenceToken& tmp_i= **i;

            if(tmp_i.type == SequenceToken::Type::PEAK_TOKEN)
            {
                break;
            }
            else if (tmp_i.type == SequenceToken::Type::AA_TOKEN and tmp_i.aa_token.pt_data!=NULL)
            {
                sequence.sequence[size-2] = &tmp_i;
                sequence.sequence[size-1] = tmp_i.aa_token.pt_data;
                res.emplace_back(sequence);
            }
            --i;
        }
    }

    void Analyser::merge_solution(std::list<Sequence>& finds,const std::list<Sequence>& left_part,const std::list<Sequence>& right_part)
    {

        auto l_end = left_part.end();
        auto r_end = right_part.end();
        auto l_begin = left_part.begin();
        auto r_begin = right_part.begin();

        /*#ifndef APPRENTISSAGE
        double tmp_values[VALUES_SIZE];
        #endif
        */

        for(auto i=l_begin; i != l_end; ++i)
        {
            if(i->sequence.size() > 1) //il y a au moins 1 AA
            {
                for(auto j=r_begin; j!= r_end; ++j)
                {
                    if(j->sequence.size() > 1) //il y a au moins 1 AA
                    {
                        finds.emplace_back(*i);
                        Sequence& new_seq=finds.back();

                        //stack_token* tmp_head = new stack_token(*tmp[0]); //copie
                        //tokens_ptr.emplace_back(tmp_head);

                        //tmp[0] = tmp_head;
                        //stack_token& i_0 = *tmp[0];
                        //stack_token& j_0 = *(*j)[0];
                        //fusion des header
                        //i_0.header_token.holds[Parser::peptide::FIN_H2O].link = j_0.header_token.holds[Parser::peptide::FIN_H2O].link;
                        //i_0.header_token.holds[Parser::peptide::FIN_H2O].to_find = j_0.header_token.holds[Parser::peptide::FIN_H2O].to_find;

                        //i_0.header_token.holds[Parser::peptide::FIN].link = j_0.header_token.holds[Parser::peptide::FIN].link;
                        //i_0.header_token.holds[Parser::peptide::FIN].to_find = j_0.header_token.holds[Parser::peptide::FIN].to_find;
                        //ajout du noyeau (header-peak(en commun)-[AA -peak]* )
                        copy((*j).sequence.begin()+1,(*j).sequence.end(),back_inserter(new_seq.sequence)); 
                        //ajout du nouveau
                        
                        //TODO
                        /*#if DEBUG & DEBUG_STATS 
                        ++calc_stats[k-1][NB_SOL_FIND];
                        #endif
                        */

                        //SCORE
                        /*#ifndef APPRENTISSAGE
                        calc_values(tmp_values,tmp,pep);
                        tmp[0]->header_token.score = calc_score(tmp_values);
                        #endif
                        */

                        //print_AA(tmp);

                        //finds.emplace_back(move(tmp));
                        ///////////////////////////////////////////

                        /*#ifndef APPRENTISSAGE
                        if(finds_max_size > 0 and ++_size > finds_max_size*5)
                        {
                            const auto& _begin = finds.begin();
                            partial_sort(_begin,_begin+finds_max_size,finds.end(),solution_gt);
                            finds.resize(finds_max_size);
                            _size = finds_max_size;
                        }
                        #endif
                        */
                        /*
                        v_tokens_ptr tmp= (*i);
                        stack_token* tmp_head = new stack_token(*tmp[0]);
                        stack_token& i_0 = *tmp_head;
                        stack_token& j_0 = *(*j)[0];
                        //fusion des header
                        i_0.header_token.holds[Parser::peptide::FIN_H2O].link = j_0.header_token.holds[Parser::peptide::FIN_H2O].link;
                        i_0.header_token.holds[Parser::peptide::FIN_H2O].to_find = j_0.header_token.holds[Parser::peptide::FIN_H2O].to_find;

                        i_0.header_token.holds[Parser::peptide::FIN].link = j_0.header_token.holds[Parser::peptide::FIN].link;
                        i_0.header_token.holds[Parser::peptide::FIN].to_find = j_0.header_token.holds[Parser::peptide::FIN].to_find;
                        //ajout du noyeau (header-peak(en commun)-[AA -peak]* )
                        copy((*j).begin()+2,(*j).end(),back_inserter(tmp)); 
                        //ajout du nouveau
                        
                        //TODO
                        #if DEBUG & DEBUG_STATS 
                        ++calc_stats[k-1][NB_SOL_FIND];
                        #endif

                        //SCORE
                        #ifndef APPRENTISSAGE
                        calc_values(tmp_values,tmp,pep);
                        i_0.header_token.score = calc_score(tmp_values);
                        #endif

                        if(_size < finds_max_size or i_0.header_token.score > finds[_size-1][0]->header_token.score)
                        {
                            tokens_ptr.emplace_back(tmp_head);//save to be delete at the end
                            finds.emplace_back(move(tmp));
                            ++_size;
                        }

                        #ifndef APPRENTISSAGE
                        if(finds_max_size > 0 and _size > finds_max_size*5)
                        {
                            const auto& _begin = finds.begin();
                            partial_sort(_begin,_begin+finds_max_size,finds.end(),solution_gt);
                            finds.resize(finds_max_size);
                            _size = finds_max_size;
                        }
                        #endif
                        */
                    }
                }
            }
        }
        

        #ifndef APPRENTISSAGE
        const auto& _begin = finds.begin();

        /*int _size = finds.size();
        if(_size<finds_max_size)
            sort(_begin,finds.end(),solution_gt);
        else
            partial_sort(_begin,_begin+finds_max_size,finds.end(),solution_gt);
        */

        /*if(_size > finds_max_size)
        {
            finds.resize(finds_max_size);
        }*/
        #endif
    };


    void Analyser::__print__(const std::vector<SequenceToken*>& tokens,std::ostream& stream)
    {
        for(SequenceToken* token : tokens)
        {
            token->__print__(stream);
        }
    }


    void Analyser::__print__(const std::list<Sequence>& sequences,std::ostream& stream)
    {
        for(const Sequence& seq : sequences)
        {
            seq.__print_AA__(stream);
            stream<<std::endl;
        }
    }
}
