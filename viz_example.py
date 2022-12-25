import _spinball
import math
import numpy as np
import viz_tool
class Baseball():
    def __init__(self):
        self.rho =  1.204
        self.r = 73/2.0/1000.0
        self.m = 145/1000.0
    def set_velocity(self, mph):
        return mph*1.609*1000/3600
    def rpm_to_rads(self, rpm_list):
        rads_list = [rpm/60.0*2*math.pi for rpm in rpm_list]
        return rads_list

class curveball(Baseball):
    def __init__(self, mph, rpm_list):
        super().__init__()
        self.init_state = [0, 0, 0, 0, 1.7, 0]
        self.omega = self.rpm_to_rads(rpm_list)
        self.init_state[1] = self.set_velocity(mph)

def viz_freedrop():
    input_list = []
    free_drop = curveball(0,[0,0,0])
    spinball = _spinball.Spinball(free_drop.m, free_drop.r, free_drop.rho)
    total_time = 1
    time_inc = 0.05
    states_data = spinball.RK4_main(total_time,time_inc, free_drop.init_state, free_drop.omega)
    print(states_data)
    freedrop_dict = dict()
    freedrop_dict['States'] = np.array(states_data)
    
    freedrop_dict['Omega'] = free_drop.omega
    input_list.append(freedrop_dict)

    viz = viz_tool.viz_tool_baseball(spinball, input_list)
    viz.viz_video(save_image=False)
    viz.viz_static()
def data_prepare(spinball, mph, rpm_list, total_time, time_inc):
    '''prepare dict for given condition'''
    ball = curveball(mph, rpm_list)   

    states_data = spinball.RK4_main(total_time,time_inc, ball.init_state, ball.omega)
    ball_dict = dict()
    ball_dict['States'] = np.array(states_data)
    ball_dict['Omega'] = ball.omega
    return ball_dict

if __name__ == '__main__':
    
    ball = Baseball()
    spinball = _spinball.Spinball(ball.m, ball.r, ball.rho, 20)
    total_time = 1
    time_inc = 0.02
    input_list = []
    ## append different initial condition of balls
    input_list.append(data_prepare(spinball, 75, [0,0,2000],total_time, time_inc))
    input_list.append(data_prepare(spinball, 75, [0,0,4000],total_time, time_inc))
    input_list.append(data_prepare(spinball, 95, [0,-2000,0],total_time, time_inc))

    viz = viz_tool.viz_tool_baseball(spinball, input_list)
    viz.viz_video(save_image=False)
    # viz2 = viz_tool.viz_tool_baseball(spinball, input_list)
    # viz2.viz_static()
    
