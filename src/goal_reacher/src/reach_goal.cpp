#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <tf/tf.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

class GoalReacher {
private:
    ros::NodeHandle nh;
    MoveBaseClient ac;
    ros::Subscriber goal_sub; // Rviz에서 설정한 목적지를 받기 위한 구독자
    ros::Subscriber amcl_sub; // 현재 위치를 받기 위한 구독자
    geometry_msgs::PoseStamped current_goal;
    bool goal_received;

public:
    GoalReacher() : ac("move_base", true), goal_received(false) {
        // Rviz에서 설정한 목적지를 받는 subscriber
        goal_sub = nh.subscribe("/move_base_simple/goal", 1, &GoalReacher::goalCallback, this);
        // 현재 위치를 받는 subscriber
        amcl_sub = nh.subscribe("/amcl_pose", 1, &GoalReacher::amclPoseCallback, this);

        while(!ac.waitForServer(ros::Duration(5.0))){
            ROS_INFO("Waiting for the move_base action server");
        }
    }

    void goalCallback(const geometry_msgs::PoseStamped::ConstPtr& msg) {
        current_goal = *msg; // Rviz에서 설정한 목적지를 current_goal에 저장
        goal_received = true; // 목적지를 받았다는 플래그 설정
        ROS_INFO("Goal received from Rviz");
    }

    void amclPoseCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg) {
        if (!goal_received) return; // 목적지가 설정되지 않았다면 반환

        double distance = getDistance(msg->pose.pose, current_goal.pose);
        if (distance < 0.1) {
            ROS_INFO("Goal reached, stopping the robot");
            ac.cancelAllGoals(); // 움직임을 종료
            goal_received = false; // 플래그 재설정
        }
    }

    double getDistance(const geometry_msgs::Pose& pose1, const geometry_msgs::Pose& pose2) {
        double dx = pose1.position.x - pose2.position.x;
        double dy = pose1.position.y - pose2.position.y;
        return sqrt(dx*dx + dy*dy);
    }
};

int main(int argc, char** argv){
    ros::init(argc, argv, "goal_reacher_cpp");
    GoalReacher goalReacher;
    ros::spin();
    return 0;
}