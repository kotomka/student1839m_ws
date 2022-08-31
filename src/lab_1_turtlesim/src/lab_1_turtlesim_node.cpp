#include <ros/ros.h>
#include <turtlesim/Color.h>
#include <std_srvs/Empty.h>
#include <turtlesim/Kill.h>
#include <turtlesim/Spawn.h>
#include <turtlesim/SetPen.h>
#include <turtlesim/Pose.h>
#include <geometry_msgs/Twist.h>

turtlesim::Pose turtlesim_pose;

void callback_leni(const turtlesim::Pose &msg){
 turtlesim_pose = msg;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "lab_1_turtlesim_node");
  ros::NodeHandle nh;

  nh.setParam("/turtlesim/background_r", 255);
  nh.setParam("/turtlesim/background_g", 0);
  nh.setParam("/turtlesim/background_b", 0);
  ros::ServiceClient clear_1 = nh.serviceClient<std_srvs::Empty>("/clear");
  std_srvs::Empty srv;
  clear_1.call(srv);

  ros::ServiceClient kill_1 = nh.serviceClient<turtlesim::Kill>("/kill");
  turtlesim::Kill kll;
  kll.request.name = "turtle1";
  while(!kill_1.call(kll)) {}

  ros::ServiceClient spawn_1 = nh.serviceClient<turtlesim::Spawn>("/spawn");
  turtlesim::Spawn spw;
  spw.request.x = 2.0;
  spw.request.y = 2.0;
  spw.request.theta = 0.0;
  spw.request.name = "leni";
  spawn_1.call(spw);

  spw.request.x = 6.0;
  spw.request.y = 6.0;
  spw.request.theta = 0.0;
  spw.request.name = "steven";
  spawn_1.call(spw);

  ros::ServiceClient setpen_1 = nh.serviceClient<turtlesim::SetPen>("/leni/set_pen");
  turtlesim::SetPen setp;
  setp.request.r = 255;
  setp.request.g = 255;
  setp.request.b = 0;
  setp.request.width = 10;
  setpen_1.call(setp);

  setpen_1 = nh.serviceClient<turtlesim::SetPen>("/steven/set_pen");
  setp.request.r = 0;
  setp.request.g = 255;
  setp.request.b = 0;
  setp.request.width = 10;
  setpen_1.call(setp);

  ros::Publisher pose_leni = nh.advertise<geometry_msgs::Twist>("/leni/cmd_vel", 1000);
  geometry_msgs::Twist  pslen;
  pslen.linear.x = 1.0;
  pslen.angular.z = 1.0;

  ros::Publisher pose_steven = nh.advertise<geometry_msgs::Twist>("/steven/cmd_vel", 1000);
  geometry_msgs::Twist  psstev;
  psstev.linear.x = 1.5;
  psstev.angular.z = 1.0;

  ros::Subscriber leniPose_sub = nh.subscribe("/leni/pose", 1000, callback_leni);

  ros::Rate r(2000);
  while (ros::ok()) {
  pose_leni.publish (pslen);
  pose_steven.publish (psstev);

//  if((turtlesim_pose.theta < 0) && (turtlesim_pose.theta > -0.05)) {
//    pslen.linear.x = 0.0;
//    psstev.linear.x = 0.0;
//    pslen.linear.y = 0.0;
//    psstev.linear.y = 0.0;
//    pose_leni.publish (pslen);
//    pose_steven.publish (psstev);
//
//     break;
//    }

  ros::spinOnce();

  r.sleep();
  }

  return 0;
}
