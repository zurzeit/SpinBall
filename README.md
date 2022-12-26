
# Spinball


## Problem to Solve

As we watch the baseball game or table tennis game, we can see the spinning ball moving with a tricky trajectory. That is because the Magnus effect gives an additional force to the ball. As a result, the ball drops rapidly.

After evaluating the free body diagram of a spinning ball, there are three external force exerting on the ball, which are gravity, drag and force from the Magnus effect. And we can get the following equations




$$\ddot{x}=\dfrac{F_{D_x}}{m}+\dfrac{F_{L_x}}{m}$$

$$\ddot{y}=\dfrac{F_{D_y}}{m}+\dfrac{F_{L_y}}{m}$$

$$\ddot{z}=\dfrac{F_{D_z}}{m}+\dfrac{F_{L_z}}{m}-g$$


$F_{D}$ is drag force and $F_{L}$ is Magnus effect force. Their relation with some fluid attributes are shown below:
$$F_{D}=\dfrac{1}{2} \rho U^{2}AC_{D}$$
$$F_{L}=\dfrac{1}{2}\rho U^2AC_{L}$$
where 
* $A$ is the cross-area of the ball, 
* $\rho$ is the fluid density, 
* $U$ is the relative velocity between the ball and the fluid,
* $C_{D}$ is the drag coefficient which is related to the reynolds number,
* $C_{L}$ is the lift coefficient which is related to the reynolds number as well.

To solve this problem, **Runge–Kutta method** is used to calculate the state of variables.

## Installation
Download the Spinball project through git.

`git clone https://github.com/zurzeit/SpinBall.git`

Create conda environment with name "spinball" and activate it.

```
conda create -n spinball python=3.9
conda activate spinball
```

Install requirements with the following command:

`make setup`

## Build the project
Use makefile to build the spinball package.
`make all`

Now, we are ready to use the Spinball functions!
## API Description

1. `import _spinball` 
    * Import the spinball package to solve the following problem.

2. `spinball_obj = _spinball.Spinball(ball_mass, ball_radius, density_of_air, max_x_distance=inf)` 
    * This function will create the Spinball instance, with the input arguments of `ball_mass` (kg), `ball_radius` (m), `density_of_air` (rad), `max_x_distance` (m). The `max_x_distance` means that the following result will only show the ball states whose x positions are still smaller than the max_x_distance.
    
3. `spinball_obj.RK4_main(total_time, time_inc, init_state, omega)`
    * Return the states using the Runge–Kutta method to calculate the positions and the velocity in a given time interval. 
        * `total_time` (sec): means the time length we want the ball states to be calculated.
        * `time_inc` (sec): means the time increment when using the Runge–Kutta method. The smaller time_inc is expected to produce a more precise result.
4. `spinball.print_states()`
    * This function will print the states info with the last calculated states.
5. `spinball_obj.states`
    * Return the list with the length of timestamps. Each list element contains a 6-element length list, which indicates the states of the moment. (e.g. `[x,vx,y,vy,z,vz]`)
6. `spinball_obj.M`
    * Return the mass of the ball.
7. `spinball_obj.R`
    * Return the radius of the ball.
8. `spinball_obj.Max_x_distance`
    * Return the limit of Max_x_distance of this spinball instance.
9. `spinball_obj.Omega`
    * Return a list, each element is the angular velocity of each axis. (i.e. [wx, wy, wz])
10. `spinball_obj.Num_state`
    * Return a int, which means the number of the state variable we calculate in this class. In this case, it is 6. (state variables: [x, vx, y, vy, z, vz])
12. `spinball_obj.Total_timestamp`
    * Return a integer, it means the total frames of the states including the initial states(i.e. t = 0).
13. `spinball_obj.Done_timestamp`
    * Return a integer, it means the index of frame which the states finish the calculation. The initial states is defined with index 0.
14. `spinball_obj.Final_timestamp`
    * Return a integer, it means the index of the end frame. The initial states is defined with index 0.
## Visualization
This section is about the visualization part. In the file "viz_example.py" showcases some of the examples.

 ![](https://github.com/zurzeit/SpinBall/blob/master/img/still_simulation.png)

 ![](https://github.com/zurzeit/SpinBall/blob/master/img/ball_simulation.gif)

Let's try to run a simulation!

`make viz`

#### Function explaination
1. `viz = viz_tool.viz_tool_baseball(spinball_obj,list_of_states_dict)`: Create the instance to prepare for the simulation.
2. `viz.viz_video()`: Play the simulation video.
3. `viz.viz_static()`: Show the final state of the simulation.

## References

1. Magnus effect Wiki: https://en.wikipedia.org/wiki/Magnus_effect
2. Table Tennis and Physics: https://www.intechopen.com/online-first/83844
3. Chapter 6 - Rotating Cylinders, Annuli, and Spheres https://www.sciencedirect.com/science/article/pii/B9780123820983000068
