#include <harpe-algo/SequenceToken.hpp>

#include <iostream>
#include <harpe-algo/Context.hpp>

namespace harpe
{
    SequenceToken::SequenceToken(): type(Type::UNKNOW)
    {
    }

    SequenceToken::SequenceToken(SequenceToken&& tmp)
    {
    }

    SequenceToken::SequenceToken(const int index,mgf::Peak* peak) //peak
    {
        this->type = Type::PEAK_TOKEN;
        peak_token.index = index;
        peak_token.pt_data = peak;
    }

    SequenceToken::SequenceToken(const int i,const double e,SequenceToken* peak) //aa
    {
        this->type=AA_TOKEN;
        aa_token.error =e;
        aa_token.index = i;
        aa_token.pt_data = peak;
    }


    void SequenceToken::__print__(std::ostream& stream)const
    {
        if (type==Type::AA_TOKEN)
        {
            stream<<"AA_TOKEN error:"<<aa_token.error<<" "<<Context::aa_tab[aa_token.index].getSlug()<<" ";
            if (aa_token.pt_data != nullptr)
                aa_token.pt_data->peak_token.pt_data->__print__(stream);
            else
                stream<<std::endl;
        }
        else if (type == Type::PEAK_TOKEN)
        {
            stream<<"PEAKS_TOKEN ";
            peak_token.pt_data->__print__(stream);
        }
    }
}
