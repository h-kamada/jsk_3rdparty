// doc/html/boost_asio/example/icmp/

#include "ping.cpp"
#include <ros/ros.h>
#include <std_msgs/Float64.h>

int main(int argc, char* argv[])
{
  try
      {
          if (argc != 2)
              {
                  std::cerr << "Usage: " << argv[0] << " <host>" << std::endl;
#if !defined(BOOST_WINDOWS)
                  std::cerr << "(You may need to run this program as root.)" << std::endl;
#endif
                  return 1;
              }

          boost::asio::io_service io_service;
          pinger p(io_service, argv[1]);

          ros::init(argc, argv, "ping");

          ros::NodeHandle n;

          ros::Publisher pub = n.advertise<std_msgs::Float64>("ping/delay", 10);

          std_msgs::Float64 msg;

          ros::Time last_time;

          while (ros::ok()) {
              io_service.run_one();

              ros::Time now = ros::Time::now();
              if ( now - last_time > ros::Duration(1) ) {
                  msg.data = p.delay;
                  pub.publish(msg);
                  ros::spinOnce();

                  last_time = now;
              }
          }

      }
  catch (std::exception& e)
      {
          std::cerr << "Exception: " << e.what() << std::endl;
      }
}
