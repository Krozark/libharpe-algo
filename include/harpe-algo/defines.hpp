#ifndef HARPE_ALGO_DEFINES_HPP
#define HARPE_ALGO_DEFINES_HPP

#define HARPE_ALGO_DEFAULT_START_PEAKS_NB 10

#include <mgf/defines.hpp>

#define HARPE_ALGO_WARNNIG(txt) std::cout<<MGF_COMMENTAIRE<<"/!\\ warnning file "<<__FILE__<<" line"<<__LINE__<<" : "<<txt<<MGF_BLANC<<std::endl;

#define HARPE_ALGO_ERROR(txt) std::cout<<MGF_ROUGE<<"/!\\ warnning file "<<__FILE__<<" line"<<__LINE__<<" : "<<txt<<MGF_BLANC<<std::endl;

#define ABS(x) (x>0?x:-x)

#endif
