#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

class States{

public:

    States()
        : Num_state(0), Total_sec{0}, Done_timestamp(0),Final_timestamp(0),Total_timestamp(0)
    { }
    void states_reset(double total_sec, double time_inc, std::vector<double> init_state){
        if(time_inc > total_sec){
            throw std::out_of_range(
		        "the Time_inc should be smaller than total_sec ");
        }
        Total_sec = total_sec;
        Time_inc = time_inc;
        Done_timestamp = 0;
        Final_timestamp = ceil(total_sec/time_inc);
        Total_timestamp = Final_timestamp + 1;
        Num_state = init_state.size();
        reset_buffer(Total_timestamp, Num_state);
        // for(size_t i = 0; i < Num_state; i ++)
            (*this)(0) = init_state;
        return;
    }
    void print_states(){
        if(Total_timestamp == 0){
            std::cout << "empty states!!!" << std::endl;
            return;
        }
        for(size_t i = 0; i < 80; i ++)
            std::cout << "=";
        std::cout << "\n";
        std::cout << "Num_state:" << Num_state << std::endl;
        std::cout << "Total_sec:" << Total_sec << std::endl;
        std::cout << "Time_inc:" << Time_inc << std::endl;
        std::cout << "Done_timestamp/Final_timestamp:" << Done_timestamp << "/" <<Final_timestamp << std::endl;
        std::cout << "State size:  " << "row:" << Total_timestamp << "  col:" << Num_state << std::endl;
        size_t k = 0;
        for(size_t i = 0; i <= Done_timestamp; i ++){
            std::cout<< "t:" << std::left << std::setw(4) << i*Time_inc << " ";
            for(size_t j = 0; j < Num_state; j ++){
                std::cout << std::left << std::setw(10) << m_buffer[i][j] << " ";
                k ++;
            }
            std::cout << "\n";
        }
    }
    std::vector<double> operator()(size_t timestamp) const{
        return m_buffer[timestamp];
}
    std::vector<double> & operator()(size_t timestamp){
        return m_buffer[timestamp];
    }
    std::vector<std::vector<double>> get_states() const{
        return m_buffer;
    }
    size_t get_Total_timestamp() const{
        return Total_timestamp;
    }
    size_t get_Done_timestamp() const{
        return Done_timestamp;
    }
    size_t get_Final_timestamp() const{
        return Final_timestamp;
    }

    size_t get_Num_state() const{
        return Num_state;
    }
    
protected:
    size_t Num_state;
    
    double Total_sec, Time_inc;
    size_t Done_timestamp, Final_timestamp, Total_timestamp;
    std::vector<std::vector<double>> m_buffer;
    void reset_buffer(size_t nrow, size_t ncol){
        m_buffer.resize(nrow,std::vector<double>(ncol,0));
        for(size_t i = 0; i < nrow; i ++){
            // m_buffer[i].resize(ncol,std::vector<double>(ncol,0));
            std::fill(m_buffer[i].begin(), m_buffer[i].end(), 0);
        }
        
    }
    
};

