#include "states.hpp"
#include "utility.hpp"
#include <fstream>
#include <limits>
class Spinball_cls: public States {

public:
    Spinball_cls(double m, double r, double rho, double max_x_distance);
    // calc the x slope
    double func0(std::vector<double> input_state){//calculate slope
        double vx = input_state.at(1);
        return vx;
    }
    // calc the vx slope
    double func1(std::vector<double> input_state){
        return drag*drag_dir.at(0)/M + lift*lift_dir.at(0)/M;
    }
    double func2(std::vector<double> input_state){
        double vy = input_state.at(3);
        return vy;
    }
    double func3(std::vector<double> input_state){
        return drag*drag_dir.at(1)/M + lift*lift_dir.at(1)/M;
    }
    double func4(std::vector<double> input_state){
        double vz = input_state.at(5);
        return vz;
    }
    double func5(std::vector<double> input_state){
        return drag*drag_dir.at(2)/M + lift*lift_dir.at(2)/M + G;
    }

    double calc_v2(std::vector<double> input_state){
        double vx = input_state.at(1);
        double vy = input_state.at(3);
        double vz = input_state.at(5);
        return vx*vx + vy*vy + vz*vz;
    }
    double L2_norm(std::vector<double> vec){
        return sqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]);
    }
    std::vector<double> normalized_vec2(std::vector<double> vec){
        double magnitude = L2_norm(vec);
        if(magnitude == 0){
            for(size_t i = 0; i < vec.size(); i ++){
                vec.at(i) = 0;
            }
            return vec;
        }
        for(size_t i = 0; i < vec.size(); i ++){
            vec.at(i) = vec.at(i)/magnitude;
        }
        return vec;
    }
    void load_Cdmap(){
        std::ifstream infile("Cdmap.txt");
        double a,b;
        while (infile >> a >> b){
            Cdmap.push_back(b);
        }
    }
    void load_Clmap(){
        std::ifstream infile("Clmap.txt");
        double a,b;
        while (infile >> a >> b){
            Clmap.push_back(b);
        }
    }
    std::pair<double, double>  get_Cd_Cl(){
        std::vector<double> tmp_vec = proj2vector(Omega, Vel_vec);
        std::vector<double> w_effective = linear_trans(Omega, tmp_vec, -1);

        double spin_rate = L2_norm(w_effective)*R/L2_norm(Vel_vec);
        std::pair<double,double> cd_cl;
        cd_cl.first = Cdmap[spin_rate_hashfx(spin_rate)];
        cd_cl.second = Clmap[spin_rate_hashfx(spin_rate)];
        return cd_cl;
    }
    void get_drag_lift(std::vector<double> input_state){
        std::pair<double,double> Cd_Cl;
        Cd_Cl = get_Cd_Cl();
        Cd = Cd_Cl.first;
        Cl = Cd_Cl.second;

        double V2 = Vel_vec[0]*Vel_vec[0] + Vel_vec[1]*Vel_vec[1] + Vel_vec[2]*Vel_vec[2];
        drag = 0.5*Rho*V2*A*Cd;
        lift = 0.5*Rho*V2*A*Cl;
        // calc drag direction
        for(size_t i = 0; i < 3; i ++){
            if(Vel_vec.at(i) == 0)
                drag_dir.at(i) = 0;
            else
                drag_dir.at(i) = -Vel_vec.at(i);
        }
        // drag_dir.at(0) = -Vel_vec.at(0);
        // drag_dir.at(1) = -Vel_vec.at(1);
        // drag_dir.at(2) = -Vel_vec.at(2);
        drag_dir = normalized_vec2(drag_dir);
        // calc lift direction
        cross_product(Omega, Vel_vec, lift_dir);
        lift_dir = normalized_vec2(lift_dir);
    }

    int spin_rate_hashfx(double spin_rate){
        if(spin_rate < 0){
            throw std::out_of_range(
		        "the spin_rate should be smaller than 0 ");
        }
        int key = (int)(spin_rate*100);
        if(key >= Cdmap.size()){
            return Cdmap.size() - 1;
        }
        return key;
    }
    std::vector<double> RK4_core(std::vector<double> init_state){
        std::vector<double> ret_state, tmp_slope, null_vec(6), input_state, prev_slope(6);
        std::vector<double> k_total(6,0);
        double h = 0, w;
        
        //calc k_x
        for(size_t i = 0; i < 4; i ++){
            if(i == 0){
                w = 1;
                h = 0;
            } else if(i == 1 || i == 2){
                w = 2;
                h = Time_inc/2;
            } else{
                w = 1;
                h = Time_inc;
            }
            
            input_state = linear_trans(init_state, prev_slope, h);
            Vel_vec.at(0) = input_state.at(1);
            Vel_vec.at(1) = input_state.at(3);
            Vel_vec.at(2) = input_state.at(5);
            get_drag_lift(input_state);
            
            tmp_slope = null_vec;
            tmp_slope.at(0) = func0(input_state);
            tmp_slope.at(1) = func1(input_state);
            tmp_slope.at(2) = func2(input_state);
            tmp_slope.at(3) = func3(input_state);
            tmp_slope.at(4) = func4(input_state);
            tmp_slope.at(5) = func5(input_state);

            prev_slope = tmp_slope;
            k_total = linear_trans(k_total, tmp_slope, w);
        }
        ret_state = linear_trans(init_state, k_total, Time_inc/6);

        return ret_state;
    }
    std::vector<std::vector<double>> RK4_main(double total_sec, double time_inc, std::vector<double> & init_state, std::vector<double> & omega)
    {
        // for(size_t i = 0; i < 80; i ++)

        states_reset(total_sec, time_inc, init_state);
        std::vector<double> input_state, ret_state;
        Omega = omega;
        input_state = (*this)(Done_timestamp);

        while((Done_timestamp < Final_timestamp) && (input_state[0] < Max_x_distance)){
            ret_state = RK4_core(input_state);
            // ret_state.resize(init_state.size(), 0);
            (*this)(Done_timestamp + 1) = ret_state;
            Done_timestamp ++;
            input_state = ret_state;
            ret_state.clear();
            // print_states();
        }
        if(Done_timestamp != Final_timestamp){
            Total_timestamp = Done_timestamp + 1;
            m_buffer.resize(Total_timestamp);
            
        }
        return m_buffer;
    }
    double get_R() const{
        return R;
    }
    std::vector<double> get_Omega() const{
        return Omega;
    }
    double get_Max_x_distance() const{
        return Max_x_distance;
    }
private:
    double M, R, Rho, Max_x_distance;
    double G = -9.18;
    double A, drag, lift, Cd, Cl;
    std::vector<double> Omega;
    std::vector<double> Cdmap, Clmap, drag_dir, lift_dir, Vel_vec;   
};
Spinball_cls::Spinball_cls(double m, double r, double rho, double max_x_distance=std::numeric_limits<double>::infinity())
    : States(), M(m), R(r), Rho(rho), Max_x_distance(max_x_distance)
{
    lift_dir.resize(3);
    drag_dir.resize(3);
    Vel_vec.resize(3);
    A = M_PI*r*r;
    Cd = 0.5;
    Cl = 0.5;
    load_Cdmap();
    load_Clmap();

}
