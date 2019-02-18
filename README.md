# CarND-Controls-MPC
---

## The Model

* The model is a Global Kinematic Model. The state includes,

  x - Vehicle position 

  y - Vehicle position 

  psi - orientation

  v - velocity

  cte - cross check error

  epsi - orientation error

* The actuators include,

  delta - steering angle

  a - acceleration, both positive and negative

* The update equations are,

  ![update_equations](C:\temp\Backup\Work\UC\Self Driving Car\Part 2 Sensor Fusion Localization\5.21 P MPC Project\CarND-MPC-Project-master\update_equations.JPG)

  In the simulator however, a positive value implies a right turn and a negative value implies a left turn, which changes the negative/positive of delta.


## Timestep Length and Elapsed Duration (N & dt)

I started with the value in lesson N = 25 dt = 0.05, which didn't produce good result, the vehicle ran out of lane quickly.  Then I checked the time elapsed between each communication with the simulator, and found it's about 0.15 second in my environment (I'm running on a VM Ubuntu).  So I changed dt = 0.15, and the vehicle ran better and further, but still cannot complete the whole lap. I tried different N values from 5 to 25, with 25 the vehicle zigzag and finally drove off the lane, with 5 the vehicle just ran off the lane with one direction quickly. I also tried other combinations like (25, 0.1), (5, 0.4), (10, 0.15), (20, 0.75), (15, 0.2). The (10, 0.15) works best and can finish the lap, which is recorded in the video fixed_dt.avi, with highest speed about 80 mph. And the T=1.5 second also makes sense.

My VM was running on a laptop, an interesting experience is, when my laptop running with battery, the passed (10, 0.15) will fail. I checked the time elapsed between each communication and it's larger, about 0.28 second. So I add a "dynamic dt" code to calculate the average of first several intervals (discard the 1st one, which is much larger), and use this average value as dt. This mechanism works very well no matter power supply plugged in or not on laptop, and it can complete one lap with highest speed about 90 mph. In real world this shall not be an issue, since the real time system can be used rather than simulator.

The worst case I tried is when the VM ran on window mode, laptop with battery, simulator with highest resolution/fantastic graphic quality and I remote connect to the laptop, in this case the interval increased to 0.69 second and the "dynamic dt" can still pass, just with much lower speed.

Furthermore, I even tried to use the time elapsed after last communication as dt, and it also worked well, just with more variable speed.  

The code I submitted is with "dynamic dt" (controlled by dynamic_dt = true in MPC.cpp), because I think it's more robust. I also include a video fixed_dt.avi with fixed N=10 dt=0.15, which completed entire lap too.

## Polynomial Fitting and MPC Preprocessing

Calculating CTE is easier in car's coordinate system, so I converted the waypoints' positions to vehicle's coordinate system.  This is done by centering with the difference of car's position, and rotating counterclockwise by psi.  

The converted points was used to fit a 3rd order polynomial, the polynomial's value at x=0 is the CTE. The epsi is calculated as ψ−ψdes.

The x, y, and psi of vehicle is converted to zero in vehicle's coordinate system. 

## Model Predictive Control with Latency

My handle of 100ms latency is very straightforward, calculating the state after the latency, and using that state as the initial state. So when the actuation take effect, the vehicle is right at the state that the actuation   is based upon.


