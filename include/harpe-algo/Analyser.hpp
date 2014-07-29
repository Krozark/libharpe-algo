#ifndef HARPE_ANALYSER_HPP
#define HARPE_ANALYSER_HPP

#include <mgf/Driver.hpp>
#include <mgf/Spectrum.hpp>

#include <harpe-algo/Sequence.hpp>

#include <iostream>
#include <list>
#include <vector>

double get_max_mem(double coef);

namespace harpe
{
    /**
     * \brief A class to analyse a MS/MS peak list and find Sequence
     */
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
            
            /**
             * \brief analyse all the MS/MS spectrum in the parsed input, and try to build the Sequence
             */
            int analyse(std::vector<SequenceToken*>& tokens_ptr);

            /**
             * \brief analyse all the MS/MS spectrum in the parsed input, and try to build the Sequence
             * If status == LearningTooMuchFindsError, the datas is not merged completly
             */
            static std::vector<harpe::Sequence> analyse(const mgf::Spectrum& s,std::vector<SequenceToken*>& tokens_ptr,int& status,int debut=-1);

            enum Status {
                Unknow = 0, ///< unknow status
                Ok, ///< no problem [default]
                Timeout, ///< more time is needed to finish
                MemoryError,//< Too much memory needed to finish the analyse
                LearningTooMuchFindsError//< too much datas are finds (in learning mod only)
            };            

            static std::string strErr(int s);///< return a message corresponding to Status code


            /**
             * \brief free all the alocated memory
             */
            static void free(std::vector<SequenceToken*>& tokens_ptr);
            
        private:
            typedef std::list<harpe::SequenceToken*> pile_tokens_ptr; ///< type of the stack
            enum Sens{LEFT=-1,STOP=0,RIGHT=1}; ///< type of the analyse direction


            /**
             * \brief get a list of the most intens peaks
             * \param spectrum the spectrum to analyse
             * \param nb the number of peak to return
             * \return the peak index list in the spectrum
             */
            static const std::vector<int> get_index_max_intensitee_vector(const mgf::Spectrum& spectrum,const unsigned int nb);

            /**
             * \brief Get a list of SequenceToken near the peak index in param
             * \param peak_list peak list to analyse (spectrum)
             * \param index of the start point
             * \param inc direction to analyse
             * \return a vector of SequenceToken
             */
            static std::vector<SequenceToken*> get_near(const std::vector<mgf::Peak*>& peak_list,const int index, const Sens inc,std::vector<SequenceToken*>& tokens_ptr);

            /**
             * \brief pop one element of the stack using the direction
             * \param search the stack
             * \param sens the direction of analyse
             * \return the new current_index value (start point). -1 if none.
             */
            static int pop_stack(pile_tokens_ptr& search,const int sens);

            /**
             * \brief save the Sequence in the stack
             * \param search the stack to save
             * \param spectrum the spectum associated
             * \param res Where the Sequence is added
             */
            static void save_stack_right(const pile_tokens_ptr& search,const mgf::Spectrum& spectrum,std::list<Sequence>& res);

            /**
             * \brief save the Sequence in the stack
             * \param search the stack to save
             * \param spectrum the spectum associated
             * \param res Where the Sequence is added
             */
            static void save_stack_left(const pile_tokens_ptr& search,const mgf::Spectrum& spectrum,std::list<Sequence>& res);

            /**
             * \brief Merge two list of sequences of différente analyse direction strating from the same peak
             * \param finds Where the sequences are added
             * \param left_part the left sequences part
             * \param right_part the right part Sequence
             * \param spectrum the related spectrum
             */
            static void merge_solution(std::vector<Sequence>& finds,const std::list<Sequence>& left_part,const std::list<Sequence>& right_part,const mgf::Spectrum& Spectrum,int& status);


            /**
             * \brief Debug print
             */
            static void __print__(const std::vector<SequenceToken*>&,std::ostream& stream);

            /**
             * \brief Debug print
             */
            static void __print__(const std::list<Sequence>&,std::ostream& stream);

            
        private:
            mgf::Driver driver; ///< the driver for the analyse (parser)
    };
}
#endif
