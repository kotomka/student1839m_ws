// pointcloud 2
//frame: cloud_1
//topic: point_cloud

#include <ros/ros.h>
#include <sensor_msgs/PointCloud.h>
#include <sensor_msgs/PointCloud2.h>
#include <geometry_msgs/Point32.h>
#include <sensor_msgs/point_cloud2_iterator.h>
#include <math.h>

static sensor_msgs::PointCloud2 pc_1;


static int width = 1;
static int height = 3;
static double scalling = 1000;

void point_cloud1();

int main(int argc, char **argv)
{
    ros::init(argc, argv, "lab_4_pointcloud_node");
    ros::NodeHandle nh;
    ros::Publisher pub_pc_1 = nh.advertise<sensor_msgs::PointCloud2>("point_cloud", 10);


    ros::Rate r(1);

    while (ros::ok()){
        point_cloud1();

        pub_pc_1.publish(pc_1);
        ROS_INFO("Publish is complete");

        pc_1.data.clear();

        ros::spinOnce();
        r.sleep();
    }
    return 0;
}


void point_cloud1(){
    pc_1.header.frame_id = "cloud_1";
    pc_1.header.stamp = ros::Time::now();
    pc_1.width = width;
    pc_1.height = height;
    pc_1.is_bigendian = false;
    pc_1.is_dense = false;

    sensor_msgs::PointCloud2Modifier modifier(pc_1);

    modifier.setPointCloud2FieldsByString(2, "xyz", "rgb");
    modifier.resize(width*scalling*scalling*height);

    sensor_msgs::PointCloud2Iterator<float> out_x(pc_1, "x");
    sensor_msgs::PointCloud2Iterator<float> out_y(pc_1, "y");
    sensor_msgs::PointCloud2Iterator<float> out_z(pc_1, "z");
    sensor_msgs::PointCloud2Iterator<uint8_t> out_r(pc_1, "r");
    sensor_msgs::PointCloud2Iterator<uint8_t> out_g(pc_1, "g");
    sensor_msgs::PointCloud2Iterator<uint8_t> out_b(pc_1, "b");

    for(double i = 0; i < 2*M_PI; i = i + 0.01){
        for(double j = 0; j < height; j = j + 0.01){




            *out_z = (j);
            *out_x = (cos(i));
            *out_y = (sin(i));
            std::cout<<"z: "<<*out_z << "  j: " << j <<" i: "<< i <<std::endl;

            if(j <1){
                *out_r = abs(255*j);
                *out_g = 0;
                *out_b = 0;

            } else if (j>=1&&j<2) {

                *out_r = 0;
                *out_g = abs(255*j/2);
                *out_b = 0;
            } else if (j >= 2) {
                *out_r = 0;
                *out_g = 0;
                *out_b = abs(255*j/3);
            }
            // point color
//            *out_r = 255;
//            *out_g = 255;
//            *out_b = 255;

            //             increment point
            ++out_x; ++out_y; ++out_z;
            ++out_r; ++out_g; ++out_b;



        }
    }
}


