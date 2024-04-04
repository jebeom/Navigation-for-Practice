## KIST SLAM & Navigation for Mobile Manipulator

I utilized the [TurtleBot3 open-source](https://github.com/ROBOTIS-GIT/turtlebot3) to perform SLAM and navigation on a new platform (Mobile Manipulator). 

During this process, I tuned parameters within nodes such as AMCL and move_base, and modified packages related to the altered platform.

---
## Quick Run 

### Gazebo Simulation
```
roslaunch turtlebot3_gazebo turtlebot3_world.launch
roslaunch turtlebot3_navigation turtlebot3_navigation.launch map_file:=/home/jebeom/kist_slam_navi/catkin_ws/src/turtlebot3/turtlebot3_navigation/maps/map.yaml
rosrun goal_reacher reach_goal
```

### Real World

to be updated
