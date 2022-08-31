//визуализация позиции и траектории черепахи
//frame: array
//topic: pose_first_turtle, trajectory_first_turtle

#include <ros/ros.h>
#include <turtlesim/Pose.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Pose2D.h>
#include <geometry_msgs/PoseArray.h>
#include <geometry_msgs/Transform.h>
#include <tf/transform_broadcaster.h>

static geometry_msgs::Pose pose1_turtle;
static geometry_msgs::Pose2D pose1_turtle_;

void callback_1_turtle(const turtlesim::Pose &msg){

    pose1_turtle.position.x = msg.x;
    pose1_turtle.position.y = msg.y;
    pose1_turtle.position.z = 1.0;

    pose1_turtle_.x = msg.x;
    pose1_turtle_.y = msg.y;
    pose1_turtle_.theta = msg.theta;
    ROS_INFO("Message recived");
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "lab_4_2_node");
    ros::NodeHandle nh;

    ros::Subscriber turtle1Vel_sub = nh.subscribe ("/leni/pose", 1000, callback_1_turtle);

    ros::Publisher turtle1Vel_pub = nh.advertise<geometry_msgs::PoseArray>("pose_first_turtle", 500);
    ros::Publisher turtle1Traj_pub = nh.advertise<geometry_msgs::PoseArray>("trajectory_first_turtle", 500);

    geometry_msgs::PoseArray traj_turtle;

    ros::Time current_time, last_time;
    current_time = ros::Time::now();
    last_time = ros::Time::now();

    ros::Rate r(10);

    while (ros::ok()) {

        current_time = ros::Time::now();

        geometry_msgs::Quaternion pose_quat =
                tf::createQuaternionMsgFromYaw(pose1_turtle_.theta);
        pose1_turtle.orientation = pose_quat;


        //Формируемсообщение, содержащее параметры координат
        geometry_msgs::PoseArray pose_turtle;

        pose_turtle.header.stamp = ros::Time::now();
        pose_turtle.header.frame_id = "array";
        pose_turtle.poses.push_back(pose1_turtle);


        traj_turtle.header.stamp = ros::Time::now();
        traj_turtle.header.frame_id = "array";
        traj_turtle.poses.push_back(pose1_turtle);



        turtle1Vel_pub.publish(pose_turtle);
        turtle1Traj_pub.publish(traj_turtle);
        last_time = current_time;
        ros::spinOnce();
        r.sleep();
    }
}
