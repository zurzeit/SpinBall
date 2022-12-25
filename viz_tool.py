import open3d as o3d
import numpy as np

import numpy as np
import open3d as o3d
import _spinball
import time
import copy
import math

import os
class viz_tool_baseball():
    def __init__(self, spinball_cls,list_of_ballstates_dict) -> None:
        self.spinball_cls = spinball_cls

        self.list_of_ball_dict = self.get_initial_and_translate2(list_of_ballstates_dict)
        self.num_of_demo_balls = len(list_of_ballstates_dict)
        self.color_list = [[i//4, (i%4)//2, (i%2)//1] for i in range(1,8)]
        
        self.target_distance = spinball_cls.Max_x_distance

        
    def get_initial_and_translate2(self, list_of_balldict):
        
        ret_list = []
        self.num_translate = 0
        for index, ball_dict in enumerate(list_of_balldict):

            positions = ball_dict['States'][:,[0,2,4]]
            init_position = positions[0,:]
            translate = positions[1:,:] - positions[:-1,:]
            if index == 0:
                self.initial = init_position
            state_dict = dict()
            state_dict['initial'] = init_position
            state_dict['translate'] = translate
            state_dict['Omega'] = ball_dict['Omega']
            state_dict['translate_len'] = translate.shape[0]
            if translate.shape[0] > self.num_translate:
                self.num_translate = translate.shape[0]
            ret_list.append(state_dict)
        
        return ret_list
    def viz_static(self):
        static_obj_list = []
        mesh_frame = o3d.geometry.TriangleMesh.create_coordinate_frame(
            size=1, origin=[0, 0, 0])
        w = self.spinball_cls.Max_x_distance
        h = 5
        d = 0.1
        mesh_floor = o3d.geometry.TriangleMesh.create_box(w,h,d)
        mesh_floor.translate((0,-h*0.5,-d))

        static_obj_list.append(mesh_frame)
        static_obj_list.append(mesh_floor)
        
        
        if not math.isinf(self.target_distance):
            for i in range(5):
                if i%2 == 0:
                    coloring = [1,0,0]
                else:
                    coloring = [1,1,1]
                thick = 0.1
                mesh_target = o3d.geometry.TriangleMesh.create_cylinder(radius=0.1*(i+1), height=thick)
                mesh_target.paint_uniform_color(coloring)
                Rotate_mat = mesh_target.get_rotation_matrix_from_xyz((0, np.pi / 2, 0 ))
                mesh_target.rotate(Rotate_mat, center=(0, 0, 0))
                mesh_target.translate((self.spinball_cls.Max_x_distance + i*0.01,self.initial[1], self.initial[2]+ 0.5*thick))
                static_obj_list.append(mesh_target)

                

        dynamic_obj_list = []
        for index, ball_dict in enumerate(self.list_of_ball_dict):
            dynamic_obj_list.append(o3d.geometry.TriangleMesh.create_sphere(radius=self.spinball_cls.R))
            dynamic_obj_list[-1].compute_vertex_normals()
            dynamic_obj_list[-1].paint_uniform_color(self.color_list[index])
            dynamic_obj_list[-1].translate(ball_dict['initial'])
            ball_dict['mesh_sphere'] = dynamic_obj_list[-1]

        for ball_dict in self.list_of_ball_dict:
            for i in range(ball_dict['translate_len']):
                ball_dict["mesh_sphere"].translate(ball_dict['translate'][i])
                shadow_obj = copy.deepcopy(ball_dict["mesh_sphere"])
                dynamic_obj_list.append(shadow_obj)
        o3d.visualization.draw(static_obj_list + dynamic_obj_list)
        
    def viz_video(self, save_image = False):
        if save_image:
            os.makedirs("image_dir",exist_ok=True)
        static_obj_list = []
        mesh_frame = o3d.geometry.TriangleMesh.create_coordinate_frame(
            size=1, origin=[0, 0, 0])
        w = self.spinball_cls.Max_x_distance
        h = self.spinball_cls.Max_x_distance
        d = 0.1
        mesh_floor = o3d.geometry.TriangleMesh.create_box(w,h,d)
        mesh_floor.translate((0,-h*0.5,-d))

        static_obj_list.append(mesh_frame)
        static_obj_list.append(mesh_floor)
        
        if not math.isinf(self.target_distance):

            for i in range(5):
                if i%2 == 0:
                    coloring = [1,0,0]
                else:
                    coloring = [1,1,1]
                thick = 0.1
                mesh_target = o3d.geometry.TriangleMesh.create_cylinder(radius=0.1*(i+1), height=thick)
                mesh_target.paint_uniform_color(coloring)
                Rotate_mat = mesh_target.get_rotation_matrix_from_xyz((0, np.pi / 2, 0 ))
                mesh_target.rotate(Rotate_mat, center=(0, 0, 0))
                mesh_target.translate((self.spinball_cls.Max_x_distance + i*0.01,self.initial[1], self.initial[2]+ 0.5*thick))
                static_obj_list.append(mesh_target)

        vis = o3d.visualization.Visualizer()
        vis.create_window(window_name='Angel Visualize', width=1600, height=1200)
        
        for mesh in static_obj_list:
            vis.add_geometry(mesh)
            vis.update_geometry(mesh)
        dynamic_obj_list = []
        for index, ball_dict in enumerate(self.list_of_ball_dict):
            
            dynamic_obj_list.append(o3d.geometry.TriangleMesh.create_sphere(radius=self.spinball_cls.R))
            dynamic_obj_list[-1].compute_vertex_normals()
            dynamic_obj_list[-1].paint_uniform_color(self.color_list[index])
            dynamic_obj_list[-1].translate(ball_dict['initial'])
            
            vis.add_geometry(dynamic_obj_list[-1])
            vis.update_geometry(dynamic_obj_list[-1])
            ball_dict['mesh_sphere'] = dynamic_obj_list[-1]
 
        ctr = vis.get_view_control()
        front_vec = np.array([-1,-0.1,0.1], dtype=np.float64)
        ctr.set_front(front_vec)
        up_vec = np.array([0,0,1], dtype=np.float64)
        ctr.set_up(up_vec)
        ctr.set_zoom(0.5)
        
        for i in range(self.num_translate):
            if i == 0:
                time.sleep(1)
            time.sleep(0.1)
            camera_translate = []
            remove_index_list = []

            for index, ball_dict in enumerate(self.list_of_ball_dict):
                if i == ball_dict['translate_len'] - 1:
                    remove_index_list.append(index)
                ball_dict["mesh_sphere"].translate(ball_dict['translate'][i])
                
                vis.update_geometry(ball_dict["mesh_sphere"])
                
                shadow_obj = copy.deepcopy(ball_dict["mesh_sphere"])
                vis.add_geometry(shadow_obj, reset_bounding_box=False)
                vis.update_geometry(shadow_obj)
                camera_translate.append(ball_dict['translate'][i][0])
            for index in remove_index_list[::-1]:
                del self.list_of_ball_dict[index]

            ctr.camera_local_translate(np.mean(camera_translate),0,0)
            vis.poll_events()
            vis.update_renderer()
            if save_image:
                vis.capture_screen_image("image_dir/temp_%04d.jpg" % i)
        time.sleep(2)
        vis.destroy_window()

        
