import _spinball
import math
import numpy as np
import viz_tool
class Baseball():
    def __init__(self):
        self.init_state = [0,75*1.609*1000/3600, 0, 0, 1.7, 0]
        self.omega = [0,0,4000/60.0*2*math.pi]
        self.total_time = 1
        self.time_inc = 0.02
        self.rho =  1.204
        self.r = 73/2.0/1000.0
        self.m = 145/1000.0
    def set_omega(self, rpm_list):
        assert len(rpm_list)== 3, "set_omega input list should be len()=3"
        self.omega = []
        for rpm in rpm_list:
            self.omega.append(rpm/60.0*2*math.pi)   
        return
    def set_velocity(self, mph):
        self.init_state[1] = mph*1.609*1000/3600
        return
def test_data():
    ball = Baseball()

    # states = _spinball.States()
    # states = states.print_states()
    spinball = _spinball.Spinball(ball.m, ball.r, ball.rho, 20)
    # double total_sec, double time_inc, std::vector<double> & init_state, std::vector<double> & omega
    a = spinball.RK4_main(ball.total_time,ball.time_inc, ball.init_state, ball.omega)
    spinball.print_states()
    ball.omega[2] = ball.omega[2]*2
    ball.init_state[1] = ball.init_state[1]*0.5
    b = spinball.RK4_main(ball.total_time*2,ball.time_inc, ball.init_state, ball.omega)
    # spinball.print_states()
    ball.init_state[1] = ball.init_state[1]*4
    c = spinball.RK4_main(ball.total_time,ball.time_inc, ball.init_state, ball.omega)
    print(spinball.states)
    print(spinball.Total_timestamp)
    print(spinball.Done_timestamp)
    print(spinball.Final_timestamp)
    print(spinball.Num_state)
    print(spinball.R)
    print(spinball.Omega)
    print(spinball.Max_x_distance)
def test_viz():
    input_list = []
    ball = Baseball()
    spinball = _spinball.Spinball(ball.m, ball.r, ball.rho, 20)
    
    ball.set_omega([0,0,2000])

    states_data = spinball.RK4_main(ball.total_time,ball.time_inc, ball.init_state, ball.omega)
    spinball.print_states()
    c2000_dict = dict()
    c2000_dict['States'] = np.array(states_data)
    c2000_dict['Omega'] = ball.omega
    input_list.append(c2000_dict)
    ball.set_omega([0,0,4000])
    
    states_data = spinball.RK4_main(ball.total_time,ball.time_inc, ball.init_state, ball.omega)
    spinball.print_states()
    c4000_dict = dict()
    c4000_dict['States'] = np.array(states_data)
    c4000_dict['Omega'] = ball.omega
    input_list.append(c4000_dict)

    ball.set_omega([0,-2000,0])
    ball.set_velocity(95)
    f2000_dict = dict()
    states_data = spinball.RK4_main(ball.total_time,ball.time_inc, ball.init_state, ball.omega)
    f2000_dict['States'] = np.array(states_data)
    f2000_dict['Omega'] = ball.omega
    spinball.print_states()
    input_list.append(f2000_dict)
    
    # ball.set_omega([0,0,0])
    # ball.set_velocity(0)
    # drop_dict = dict()
    # states_data = spinball.RK4_main(ball.total_time,ball.time_inc, ball.init_state, ball.omega)
    # drop_dict['States'] = np.array(states_data)
    # drop_dict['Omega'] = ball.omega
    # spinball.print_states()
    # input_list.append(drop_dict)
    

    # states_data = np.array(states_data)
    viz = viz_tool.viz_tool_baseball(spinball, input_list)
    viz.viz_video(save_image=True)
    # viz.viz_static()
if __name__ == '__main__':
    # test_data()
    test_viz()    
