#include "ros/ros.h"
#include "turtlesim/Kill.h"
#include "turtlesim/Color.h"
#include "std_srvs/Empty.h"
#include "turtlesim/Spawn.h"
#include "turtlesim/SetPen.h"
#include "turtlesim/Pose.h"
#include "geometry_msgs/Twist.h"
#include "iostream"


turtlesim::Pose turtlesim_pose;

//Получение позиции черепахи 1
void callback_pose(const turtlesim::Pose &msg){
    turtlesim_pose = msg;
    ROS_INFO("First turtle pose received");
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "lab2_sim_launch_node");
    ros::NodeHandle nh;

    int background;
   // nh.getParam("/turtlesim/background_b", background);

//   ROS_INFO("Waiting 1");
//    while ((background != 255)){
//        ROS_INFO("Waiting launch turtlesim_node");
//    }

    //change color
    nh.setParam("/turtlesim_node/background_b", 0);
    nh.setParam("/turtlesim_node/background_g", 0);
    nh.setParam("/turtlesim_node/background_r", 255);
    ros::ServiceClient clear_srv = nh.serviceClient<std_srvs::Empty>("/clear");
    std_srvs::Empty srv;
    clear_srv.call(srv);


    ros::ServiceClient kill_srv = nh.serviceClient<turtlesim::Kill>("/kill");
    turtlesim::Kill kill_obj;
    kill_obj.request.name = "turtle1";

    while(!kill_srv.call(kill_obj)) {}

    //spawn first turtle
    ros::ServiceClient spawn_srv = nh.serviceClient<turtlesim::Spawn>("/spawn");
    turtlesim::Spawn spawn_first_obj;

    if(nh.hasParam("name_first_tutrle")){
        nh.getParam("name_first_tutrle", spawn_first_obj.request.name);
    }
    else {spawn_first_obj.request.name = "leni";}

    if(nh.hasParam("x_pos_first_tutrle")){
        nh.getParam("x_pos_first_tutrle", spawn_first_obj.request.x);
    }
    else {spawn_first_obj.request.x = 2.0;}

    if(nh.hasParam("y_pos_first_tutrle")){
        nh.getParam("y_pos_first_tutrle", spawn_first_obj.request.y);
    }
    else {spawn_first_obj.request.y = 2.0;}

    if(nh.hasParam("th_pos_first_tutrle")){
        nh.getParam("th_pos_first_tutrle", spawn_first_obj.request.theta);
    }
    else {spawn_first_obj.request.theta = 0.0;}

    spawn_srv.call(spawn_first_obj);

    //set pen first turtle
    ros::ServiceClient setpen_first_srv = nh.serviceClient<turtlesim::SetPen>(spawn_first_obj.request.name + "/setpen");
    turtlesim::SetPen setpen_first_obj;

    if(nh.hasParam("set_pen_b_first_turtle")){
        int color_b;
        nh.getParam("set_pen_b_first_turtle", color_b);
        setpen_first_obj.request.b = color_b;
    }
    else {setpen_first_obj.request.b = 0;}

    if(nh.hasParam("set_pen_g_first_turtle")){
        int color_g;
        nh.getParam("set_pen_g_first_turtle", color_g);
        setpen_first_obj.request.g = color_g;
    }
    else {setpen_first_obj.request.g = 255;}

    if(nh.hasParam("set_pen_r_first_turtle")){
        int color_r;
        nh.getParam("set_pen_r_first_turtle", color_r);
        setpen_first_obj.request.r = color_r;
    }
    else {setpen_first_obj.request.r = 255;}

    if(nh.hasParam("set_pen_width_first_turtle")){
        int width;
        nh.getParam("set_pen_width_first_turtle", width);
        setpen_first_obj.request.width = width;
    }
    else {setpen_first_obj.request.width = 10;}

    setpen_first_srv.call(setpen_first_obj);


    //spawn second turtle
    turtlesim::Spawn spawn_second_obj;

    if(nh.hasParam("name_second_tutrle")){
        nh.getParam("name_second_tutrle", spawn_second_obj.request.name);
    }
    else {spawn_second_obj.request.name = "steven";}

    if(nh.hasParam("x_pos_second_tutrle")){
        nh.getParam("x_pos_second_tutrle", spawn_second_obj.request.x);
    }
    else {spawn_second_obj.request.x = 6.0;}

    if(nh.hasParam("y_pos_second_tutrle")){
        nh.getParam("y_pos_second_tutrle", spawn_second_obj.request.y);
    }
    else {spawn_second_obj.request.y = 6.0;}

    if(nh.hasParam("th_pos_second_tutrle")){
        nh.getParam("th_pos_second_tutrle", spawn_second_obj.request.theta);
    }
    else {spawn_second_obj.request.theta = 0.0;}
    ros::ServiceClient setpen_second_srv = nh.serviceClient<turtlesim::SetPen>(spawn_second_obj.request.name + "/set_pen");

    spawn_srv.call(spawn_second_obj);

        turtlesim::SetPen setpen_second_obj;
    //set pen second turtle
    if(nh.hasParam("set_pen_b_second_turtle")){
        int color_b;
        nh.getParam("set_pen_b_second_turtle", color_b);
        setpen_second_obj.request.b = color_b;
    }
    else {setpen_second_obj.request.b = 0;}

    if(nh.hasParam("set_pen_g_second_turtle")){
        int color_g;
        nh.getParam("set_pen_g_second_turtle", color_g);
        setpen_second_obj.request.g = color_g;
    }
    else {setpen_second_obj.request.g = 255;}

    if(nh.hasParam("set_pen_r_second_turtle")){
        int color_r;
        nh.getParam("set_pen_r_second_turtle", color_r);
        setpen_second_obj.request.r = color_r;
    }
    else {setpen_second_obj.request.r = 255;}

    if(nh.hasParam("set_pen_width_second_turtle")){
        int width;
        nh.getParam("set_pen_width_second_turtle", width);
        setpen_second_obj.request.width = width;
    }
    else {setpen_second_obj.request.width = 10;}

    setpen_second_srv.call(setpen_second_obj);



    ros::Publisher pose_leni = nh.advertise<geometry_msgs::Twist>(spawn_first_obj.request.name +  "/cmd_vel", 1000);
    geometry_msgs::Twist moveCircle_leni;
    double radius_first;
    if(nh.hasParam("radius_first")){
            nh.getParam("radius_first", radius_first);
        }
        else {radius_first = 1;}

    moveCircle_leni.linear.x = radius_first;
    moveCircle_leni.angular.z = 1.0;

    ros::Publisher pose_steven = nh.advertise<geometry_msgs::Twist>(spawn_second_obj.request.name + "/cmd_vel", 1000);
    geometry_msgs::Twist moveCircle_steven;
        double radius_second;
    if(nh.hasParam("radius_second")){
            nh.getParam("radius_second", radius_second);
        }
        else {radius_second = 1;}
    moveCircle_steven.linear.x = radius_second;
    moveCircle_steven.angular.z = 1.0;


    ros::Subscriber leniPose_sub = nh.subscribe (spawn_first_obj.request.name + "/pose", 1000, callback_pose);

    ros::Rate r(20);

    while (ros::ok()) {
        pose_leni.publish(moveCircle_leni);
        pose_steven.publish(moveCircle_steven);

        std::cout<<turtlesim_pose.x<<std::endl;

        if((turtlesim_pose.theta < 0) && (turtlesim_pose.theta > -0.05)) {
          moveCircle_leni.linear.x = 0.0;
          moveCircle_steven.linear.x = 0.0;
          moveCircle_leni.linear.y = 0.0;
          moveCircle_steven.linear.y = 0.0;
          pose_leni.publish (moveCircle_leni);
          pose_steven.publish (moveCircle_steven);

           break;
          }


        ros::spinOnce();
        r.sleep();
    }


    return 0;
}
