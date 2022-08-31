//отрисовка черепах из turtlesim в rviz

#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <turtlesim/Pose.h>
#include <geometry_msgs/Transform.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Pose2D.h>
#include "turtlesim/Kill.h"
#include "turtlesim/Color.h"
#include "std_srvs/Empty.h"
#include "turtlesim/Spawn.h"
#include "turtlesim/SetPen.h"
#include "turtlesim/Pose.h"
#include "geometry_msgs/Twist.h"
#include "iostream"

static geometry_msgs::Pose pose1_turtle;
static geometry_msgs::Pose2D pose1_turtle_;

static geometry_msgs::Pose pose2_turtle;
static geometry_msgs::Pose2D pose2_turtle_;

void leni_callback(const turtlesim::Pose &msg){
    pose1_turtle.position.x = msg.x;
    pose1_turtle.position.y = msg.y;
    pose1_turtle.position.z = 0.0;

    pose1_turtle_.x = msg.x;
    pose1_turtle_.y = msg.y;
    pose1_turtle_.theta = msg.theta;
    ROS_INFO("Message recived");

}

void steven_callback(const turtlesim::Pose &msg_2){
    pose2_turtle.position.x = msg_2.x;
    pose2_turtle.position.y = msg_2.y;
    pose2_turtle.position.z = 0.0;

    pose2_turtle_.x = msg_2.x;
    pose2_turtle_.y = msg_2.y;
    pose2_turtle_.theta = msg_2.theta;
    ROS_INFO("Message recived");

}

int main( int argc, char** argv )
{
    ros::init(argc, argv, "lab6_follow_me_node");
    ros::NodeHandle nh;
    //Частота
    ros::Time current_time, last_time;
    current_time = ros::Time::now();
    last_time = ros::Time::now();
    nh.setParam("/turtlesim/background_b", 0);
    nh.setParam("/turtlesim/background_g", 0);
    nh.setParam("/turtlesim/background_r", 255);
    ros::ServiceClient clear_srv = nh.serviceClient<std_srvs::Empty>("/clear");
    std_srvs::Empty srv;
    clear_srv.call(srv);

    ros::ServiceClient kill_srv = nh.serviceClient<turtlesim::Kill>("/kill");
    turtlesim::Kill kill_obj;
    kill_obj.request.name = "turtle1";

    while(!kill_srv.call(kill_obj)) {}

    ros::ServiceClient spawn_srv = nh.serviceClient<turtlesim::Spawn>("/spawn");
    turtlesim::Spawn spawn_obj;

    spawn_obj.request.x = 2.0;
    spawn_obj.request.y = 2.0;
    spawn_obj.request.theta = 0.0;
    spawn_obj.request.name = "leni";
    spawn_srv.call(spawn_obj);

    spawn_obj.request.x = 6.0;
    spawn_obj.request.y = 6.0;
    spawn_obj.request.theta = 0.0;
    spawn_obj.request.name = "steven";
    spawn_srv.call(spawn_obj);

    ros::ServiceClient setpen_leni_srv = nh.serviceClient<turtlesim::SetPen>("/leni/set_pen");
    turtlesim::SetPen setpen_obj;
    setpen_obj.request.b = 0;
    setpen_obj.request.g = 255;
    setpen_obj.request.r = 255;
    setpen_obj.request.width = 10;
    setpen_leni_srv.call(setpen_obj);

    ros::ServiceClient setpen_steven_srv = nh.serviceClient<turtlesim::SetPen>("/steven/set_pen");
    setpen_obj.request.b = 0;
    setpen_obj.request.g = 255;
    setpen_obj.request.r = 0;
    setpen_obj.request.width = 10;
    setpen_steven_srv.call(setpen_obj);

    ros::Publisher pose_leni = nh.advertise<geometry_msgs::Twist>("/leni/cmd_vel", 1000);


    ros::Publisher pose_steven = nh.advertise<geometry_msgs::Twist>("/steven/cmd_vel", 1000);




    ros::Publisher marker_pub =
            nh.advertise<visualization_msgs::Marker>("visualization_marker_leni", 10);
    ros::Subscriber leni_sub = nh.subscribe("/leni/pose",100, leni_callback);

    ros::Publisher marker_pub_2 =
            nh.advertise<visualization_msgs::Marker>("visualization_marker_steven", 10);
    ros::Subscriber steven_sub = nh.subscribe("/steven/pose",100, steven_callback);


    ros::Rate r(10);
    while (ros::ok())
    {
        geometry_msgs::Twist moveCircle_leni;
        moveCircle_leni.linear.x = 1.0;
        moveCircle_leni.angular.z = 1.0;

        geometry_msgs::Twist moveCircle_steven;
        moveCircle_steven.linear.x = 2.0;
        moveCircle_steven.angular.z = 1.0;

        pose_leni.publish(moveCircle_leni);
        pose_steven.publish(moveCircle_steven);

        //Объект маркера
        visualization_msgs::Marker marker;
        marker.header.frame_id = "base_link";
        marker.header.stamp = ros::Time::now();
        //Пространство имён и идентификационный номер
        marker.ns = "basic_shapes";
        marker.id = 1;
        // Тип маркера
        marker.type = visualization_msgs::Marker::MESH_RESOURCE;
        marker.mesh_resource = "file:///home/pinton/Documents/ros_labs/student1839m_ws/src/lab6_follow_me/src/Turtle.obj";
        //Действие
        marker.action = visualization_msgs::Marker::ADD;
        //Положение маркера
        marker.pose.position.x = pose1_turtle.position.x;
        marker.pose.position.y = pose1_turtle.position.y;
        marker.pose.position.z = pose1_turtle.position.z;

        geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(pose1_turtle_.theta + M_PI_2);
        marker.pose.orientation.x = odom_quat.x;
        marker.pose.orientation.y = odom_quat.y;
        marker.pose.orientation.z = odom_quat.z;

        marker.pose.orientation.w = odom_quat.w;

        //Масштаб маркера
        marker.scale.x = 0.10;
        marker.scale.y = 0.10;
        marker.scale.z = 0.1;
        //Цвет маркера
        marker.color.r = 1.0f;
        marker.color.g = 1.0f;
        marker.color.b = 0.0f;
        marker.color.a = 1.0;
        //Нулевое значение времени жизни


        visualization_msgs::Marker marker_2;
        marker_2.header.frame_id = "base_link";
        marker_2.header.stamp = ros::Time::now();
        //Пространство имён и идентификационный номер
        marker_2.ns = "basic_shapes";
        marker_2.id = 2;
        // Тип маркера
        marker_2.type = visualization_msgs::Marker::MESH_RESOURCE;
        marker_2.mesh_resource = "file:///home/pinton/Documents/ros_labs/student1839m_ws/src/lab6_follow_me/src/Turtle.obj";
        //Действие
        marker_2.action = visualization_msgs::Marker::ADD;
        //Положение маркера
        marker_2.pose.position.x = pose2_turtle.position.x;
        marker_2.pose.position.y = pose2_turtle.position.y;
        marker_2.pose.position.z = pose2_turtle.position.z;

        geometry_msgs::Quaternion odom_quat_2 = tf::createQuaternionMsgFromYaw(pose2_turtle_.theta + M_PI_2);
        marker_2.pose.orientation.x = odom_quat_2.x;
        marker_2.pose.orientation.y = odom_quat_2.y;
        marker_2.pose.orientation.z = odom_quat_2.z;
        marker_2.pose.orientation.w = odom_quat_2.w;

        //Масштаб маркера
        marker_2.scale.x = 0.10;
        marker_2.scale.y = 0.10;
        marker_2.scale.z = 0.1;
        //Цвет маркера
        marker_2.color.r = 0.0f;
        marker_2.color.g = 1.0f;
        marker_2.color.b = 0.0f;
        marker_2.color.a = 1.0;
        //Нулевое значение времени жизни
        marker_2.lifetime = ros::Duration();
        marker.lifetime = ros::Duration();
        //Публикуем

        marker_pub.publish(marker);
        marker_pub_2.publish(marker_2);


        ros::spinOnce();
        r.sleep();
    }
}
