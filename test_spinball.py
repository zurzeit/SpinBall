import _spinball
import math
import numpy as np
import unittest

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
class GradingTest(unittest.TestCase):

    def make_spinball(self, m, r, rho):
        spinball = _spinball.Spinball(m, r, rho)
        return spinball
    def test_basic(self):
        c2000 = curveball(75, [0,0,2000])
        spinball = self.make_spinball(c2000.m, c2000.r, c2000.rho)
        total_time = 1
        time_inc = 0.1
        states = spinball.RK4_main(total_time, time_inc, c2000.init_state, c2000.omega)
        spinball.print_states()
        ## check if the size is correct
        assert np.array(states).shape[0] == math.ceil(total_time/time_inc) + 1
        assert np.array(states).shape[1] == len(c2000.init_state)
        c4000 = curveball(75, [0,0,4000])
        spinball = self.make_spinball(c4000.m, c4000.r, c4000.rho)
        total_time = 1
        time_inc = 0.1
        states2 = spinball.RK4_main(total_time, time_inc, c4000.init_state, c4000.omega)
        ## check if the higher spinrate cause bigger deviation
        assert np.array(states)[-1,2] <= np.array(states2)[-1,2]
        # print(np.array(states))
        # print(np.array(states2))
    def test_spineffect(self):
        ''' check if the higher spinrate cause bigger deviation'''
        c2000 = curveball(75, [0,0,2000])
        spinball = self.make_spinball(c2000.m, c2000.r, c2000.rho)
        total_time = 1
        time_inc = 0.1
        states = spinball.RK4_main(total_time, time_inc, c2000.init_state, c2000.omega)
        spinball.print_states()
        ## check if the size is correct
        
        c4000 = curveball(75, [0,0,4000])
        spinball = self.make_spinball(c4000.m, c4000.r, c4000.rho)
        total_time = 1
        time_inc = 0.1
        states2 = spinball.RK4_main(total_time, time_inc, c4000.init_state, c4000.omega)
        
        assert np.array(states)[-1,2] <= np.array(states2)[-1,2]
    def test_velocity(self):
        ''' check if the higher velocity ball travels farther'''
        c75 = curveball(75, [0,0,0])
        spinball = self.make_spinball(c75.m, c75.r, c75.rho)
        total_time = 1
        time_inc = 0.1
        states1 = spinball.RK4_main(total_time, time_inc, c75.init_state, c75.omega)
        spinball.print_states()
        
        c100 = curveball(100, [0,0,0])
        spinball = self.make_spinball(c100.m, c100.r, c100.rho)
        total_time = 1
        time_inc = 0.1
        states2 = spinball.RK4_main(total_time, time_inc, c100.init_state, c100.omega)
        assert np.array(states1)[-1,0] <= np.array(states2)[-1,0]

    def test_freedrop(self):
        '''check if the freedrop ball doesn't have much movement in x and y axis
            and check if the ball movement is smaller than the one with ideal 
            no air drag force condition in z axis
        '''
        drop_ball = curveball(0,[0,0,0])
        spinball = self.make_spinball(drop_ball.m, drop_ball.r, drop_ball.rho)
        total_time = 5
        time_inc = 0.1
        states1 = spinball.RK4_main(total_time, time_inc, drop_ball.init_state, drop_ball.omega)
        spinball.print_states()
        assert np.array(states1)[-1,4] - np.array(states1)[0,4] < 1/2*9.18*5*5
        assert abs(np.array(states1)[-1,0]) < 0.001
        assert abs(np.array(states1)[-1,2]) < 0.001

