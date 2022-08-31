//frame: base_link

#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <interactive_markers/menu_handler.h>
#include <visualization_msgs/InteractiveMarker.h>
#include <interactive_markers/interactive_marker_server.h>
#include <turtlesim/Pose.h>
#include <tf/tf.h>
#include <geometry_msgs/Point.h>
#include <math.h>

visualization_msgs::Marker marker;
visualization_msgs::Marker second_marker;
visualization_msgs::Marker flag_marker;
visualization_msgs::InteractiveMarker main_marker;

geometry_msgs::Pose point_flag_marker;
bool flag = false, flag1 = false, flag2 = false;

void first_choise( const
              visualization_msgs::InteractiveMarkerFeedbackConstPtr &feedback ) {
    ROS_INFO("The first activated.");
    flag1 = true;
    flag2 = false;
}
void second_choise( const
               visualization_msgs::InteractiveMarkerFeedbackConstPtr &feedback )
{
    ROS_INFO("The second activated");
    flag2 = true;
    flag1 = false;
}

void processFeedback(const visualization_msgs::InteractiveMarkerFeedbackConstPtr &feedback ) {
    ROS_INFO_STREAM( feedback->marker_name << " is now at "<<feedback->pose.position.x<<","<<feedback->pose.position.y<< ", " << feedback->pose.position.z );
    point_flag_marker = feedback->pose;
    ROS_INFO_STREAM( "Point" << " is now at "<<point_flag_marker.position.x<<","<<point_flag_marker.position.y<< ", " << point_flag_marker.position.z );
    flag = true;
}



int main( int argc, char** argv )
{
    ros::init(argc, argv, "marker_test");
    ros::NodeHandle n;
    interactive_markers::InteractiveMarkerServer server("simple_marker","",false);
    interactive_markers::MenuHandler menu_handler;
    interactive_markers::MenuHandler::EntryHandle h_first_entry;
    interactive_markers::MenuHandler::EntryHandle h_second_entry;

    h_first_entry = menu_handler.insert("First turtle", &first_choise);
    h_second_entry = menu_handler.insert("Second turtle", &second_choise);

    ros::Rate r(100);

    ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 10);
    ros::Publisher second_marker_pub = n.advertise<visualization_msgs::Marker>("vtoroyVisualization_marker", 10);


    main_marker.header.frame_id = "base_link";
    main_marker.header.stamp=ros::Time::now();
    main_marker.name = "marker";
    main_marker.scale = 1;
    main_marker.description = "Click me to choose mode";


    second_marker.header.frame_id = "base_link";
    second_marker.ns = "basic_shapes";
    second_marker.id = 0;
    second_marker.type = visualization_msgs::Marker::MESH_RESOURCE;
    second_marker.mesh_use_embedded_materials=true;
    second_marker.mesh_resource="file:///home/anton/Documents/turtle_1839m_ws/src/lab_6_1_beacon/src/Turtle.obj";
    second_marker.action = visualization_msgs::Marker::MODIFY;
    second_marker.pose.position.x = 0;
    second_marker.pose.position.y = 0;
    second_marker.pose.position.z = 0;

    tf::Quaternion second_marker_quat = tf::createQuaternionFromRPY(M_PI_2,0,M_PI_2);
    second_marker.pose.orientation.w = second_marker_quat.w();
    second_marker.pose.orientation.x = second_marker_quat.x();
    second_marker.pose.orientation.y = second_marker_quat.y();
    second_marker.pose.orientation.z = second_marker_quat.z();
    second_marker.scale.x = 0.1;
    second_marker.scale.y = 0.1;
    second_marker.scale.z = 0.1;

    second_marker.color.r = 0.0f;
    second_marker.color.g = 1.0f;
    second_marker.color.b = 0.0f;
    second_marker.color.a = 1.0;

    marker.header.frame_id = "base_link";
    marker.ns = "basic_shapes";
    marker.id = 0;
    marker.type = visualization_msgs::Marker::MESH_RESOURCE;
    marker.mesh_use_embedded_materials=true;
    marker.mesh_resource="file:///home/anton/Documents/turtle_1839m_ws/src/lab_6_1_beacon/src/Turtle.obj";
    marker.action = visualization_msgs::Marker::MODIFY;

    marker.pose.position.x = 0;
    marker.pose.position.y = 0;
    marker.pose.position.z = 0;
    tf::Quaternion quat = tf::createQuaternionFromRPY(M_PI_2,0,M_PI_2);
    marker.pose.orientation.w = quat.w();
    marker.pose.orientation.x = quat.x();
    marker.pose.orientation.y = quat.y();
    marker.pose.orientation.z = quat.z();
    marker.scale.x = 0.1;
    marker.scale.y = 0.1;
    marker.scale.z = 0.1;

    marker.color.r = 1.0f;
    marker.color.g = 1.0f;
    marker.color.b = .0f;
    marker.color.a = 1.0;

    visualization_msgs::Marker flag_marker;
    flag_marker.type = visualization_msgs::Marker::MESH_RESOURCE;
    flag_marker.mesh_resource="file:///home/anton/Documents/turtle_1839m_ws/src/lab_6_1_beacon/src/beacon.obj";
    flag_marker.scale.x = 0.01;
    flag_marker.scale.y = 0.01;
    flag_marker.scale.z = 0.01;
    flag_marker.color.r = 1.0;
    flag_marker.color.g = 1.0;
    flag_marker.color.b = 0.5;
    flag_marker.color.a = 1.0;
    tf::Quaternion flag_marker_quat;
    flag_marker_quat = tf::createQuaternionFromRPY(M_PI,0,M_PI_2);
    flag_marker.pose.orientation.w = flag_marker_quat.w();
    flag_marker.pose.orientation.x = flag_marker_quat.x();
    flag_marker.pose.orientation.y = flag_marker_quat.y();
    flag_marker.pose.orientation.z = flag_marker_quat.z();


    visualization_msgs::InteractiveMarkerControl flag_control;
    flag_control.interaction_mode = visualization_msgs::InteractiveMarkerControl::MOVE_ROTATE;
    flag_control.always_visible = true;
    flag_control.markers.push_back( flag_marker );

    visualization_msgs::InteractiveMarkerControl move_control;
    move_control.name = "move";
    move_control.always_visible = true;
    move_control.interaction_mode = visualization_msgs::InteractiveMarkerControl::MOVE_ROTATE;

    main_marker.controls.push_back(move_control);

    main_marker.controls.push_back( flag_control );
    server.insert(main_marker, &processFeedback);
    menu_handler.apply(server,"marker");

    server.applyChanges();

    while (ros::ok())
    {
        marker.header.stamp = ros::Time::now();

        double delta_y;
        double delta_z;
        if(flag1)
        {
            if(abs(point_flag_marker.position.z-marker.pose.position.z)>0.5
                    || abs(point_flag_marker.position.y-marker.pose.position.y)>0.5)
            {
                ROS_INFO("Move_first");
                delta_y = (point_flag_marker.position.y-marker.pose.position.y);

                delta_z = (point_flag_marker.position.z-marker.pose.position.z);
                marker.pose.position.y+=delta_y*0.01;
                marker.pose.position.z+=delta_z*0.01;
                double alpha = atan(delta_z/delta_y);
                tf::Quaternion qu;
                if(delta_y <= 0)
                {
                    qu = tf::createQuaternionFromRPY(M_PI_2,M_PI_2-alpha,M_PI_2);
                }
                else
                {
                    qu = tf::createQuaternionFromRPY(M_PI_2,-M_PI_2-alpha,M_PI_2);
                }
                marker.pose.orientation.w = qu.w();
                marker.pose.orientation.x = qu.x();
                marker.pose.orientation.y = qu.y();
                marker.pose.orientation.z = qu.z();

                flag = false;
            }
        }
        if(flag2)
        {
            if(abs(point_flag_marker.position.z-second_marker.pose.position.z)>0.5
                    || abs(point_flag_marker.position.y-second_marker.pose.position.y)>0.5)
            {
                  ROS_INFO("Move_second");
                delta_y = (point_flag_marker.position.y-second_marker.pose.position.y);

                delta_z = (point_flag_marker.position.z-second_marker.pose.position.z);
                second_marker.pose.position.y+=delta_y*0.01;
                second_marker.pose.position.z+=delta_z*0.01;
                double alpha = atan(delta_z/delta_y);
                tf::Quaternion qu;
                if(delta_y <= 0)
                {
                    qu = tf::createQuaternionFromRPY(M_PI_2,M_PI_2-alpha,M_PI_2);
                }
                else
                {
                    qu = tf::createQuaternionFromRPY(M_PI_2,-M_PI_2-alpha,M_PI_2);
                }
                second_marker.pose.orientation.w = qu.w();
                second_marker.pose.orientation.x = qu.x();
                second_marker.pose.orientation.y = qu.y();
                second_marker.pose.orientation.z = qu.z();

                flag = false;
            }
        }

        marker_pub.publish(marker);
        second_marker_pub.publish(second_marker);
        marker.lifetime = ros::Duration();
        ros::spinOnce();
        r.sleep();
    }
}

