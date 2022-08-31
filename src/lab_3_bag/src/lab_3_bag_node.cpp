#include <ros/ros.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <turtlesim/Pose.h>

static double path_1 = 0,
path_2 = 0,
x_last = 0,
y_last = 0,
x2_last = 0,
y2_last = 0;

static bool flag_1 = true, flag_2 = true;


int main(int argc, char **argv)
{
    ros::init(argc, argv, "lab3_bagfile_node");
    ros::NodeHandle nh;




    //Открываем файл с данными для чтения
    rosbag::Bag bag;
    bag.open("/home/anton/Documents/turtle_1839m_ws/src/lab_3_bag/src/first_turtle_2022-06-12-13-38-52.bag",rosbag::bagmode::Read);

    //Определяем топики, которые будем считывать
    std::vector<std::string> topics;
    topics.push_back(std::string("/lemmi/pose"));
    //    topics.push_back(std::string("numbers"));

    //Определяем конфигурацию для считывания нескольких топиков
    rosbag::View view(bag, rosbag::TopicQuery(topics));

    //считываем все имеющиеся данные и выводим в консоль
    for(rosbag::MessageInstance const m: rosbag::View(bag))
    {
        turtlesim::Pose::ConstPtr first_turtle = m.instantiate<turtlesim::Pose>();
        if (first_turtle != NULL){
            if(flag_1){
                x_last = first_turtle ->x;
                y_last = first_turtle ->y;
                flag_1 = false;
            }
            std::cout <<"X: "<< first_turtle -> x<<std::endl;
            std::cout <<"Y: "<< first_turtle -> y<<std::endl;
            path_1 += sqrt((first_turtle->x - x_last)*(first_turtle->x - x_last)+(first_turtle->y - y_last)*(first_turtle->y - y_last));

            x_last = first_turtle ->x;
            y_last = first_turtle ->y;
        }

    }
    bag.close();

    rosbag::Bag bag2;
    bag2.open("/home/anton/Documents/turtle_1839m_ws/src/lab_3_bag/src/second_turtle_2022-06-12-13-35-11.bag", rosbag::bagmode::Read);

    //Определяем топики, которые будем считывать
    std::vector<std::string> topics2;
    topics2.push_back(std::string("/memmi/pose"));

    //Определяем конфигурацию для считывания нескольких топиков
    rosbag::View view2(bag2, rosbag::TopicQuery(topics2));

    //считываем все имеющиеся данные и выводим в консоль
    for(rosbag::MessageInstance const m: rosbag::View(bag2))
    {

        turtlesim::Pose::ConstPtr second_turtle = m.instantiate<turtlesim::Pose>();
        if (second_turtle != NULL){
            if (flag_2){
                x2_last = second_turtle ->x;
                y2_last = second_turtle ->y;
                flag_2 = false;
            }
            std::cout <<"X: "<< second_turtle -> x<<std::endl;
            std::cout <<"Y: "<< second_turtle -> y<<std::endl;
            path_2 += sqrt((second_turtle->x - x2_last)*(second_turtle->x - x2_last)+(second_turtle->y - y2_last)*(second_turtle->y - y2_last));

            //            path_1 = sqrt ((poseStamped.pose.position.x - x_last)*(poseStamped.pose.position.x - x_last)+ (poseStamped.pose.position.y-y_last)*(poseStamped.pose.position.y-y_last));
            x2_last = second_turtle ->x;
            y2_last = second_turtle ->y;
        }
    }
    std::cout<<"Total path first turtle is: "<< path_1<<std::endl;
    std::cout<<"Total path second turtle is: "<< path_2<<std::endl;
    bag2.close();
}
