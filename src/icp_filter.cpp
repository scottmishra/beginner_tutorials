#include <iostream>

#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>

#include <pcl_ros/point_cloud.h>
#include <pcl_ros/io/pcd_io.h>
#include <pcl/registration/icp.h>

typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;
ros::Publisher pub;

void callback(const PointCloud::ConstPtr& cloud_in){
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_out (new pcl::PointCloud<pcl::PointXYZ>);

  //set cloud out as cloud in
  *cloud_out = *cloud_in;
  std::cout << "size:" << cloud_out->points.size() << std::endl;
 //tranform cloud out compared to cloud in
  for (size_t i = 0; i < cloud_in->points.size (); ++i)
    cloud_out->points[i].x = cloud_in->points[i].x + 0.7f;
  std::cout << "Transformed " << cloud_in->points.size () << " data points:" << std::endl;

  //setup an icp object
  pcl::IterativeClosestPoint<pcl::PointXYZ, pcl::PointXYZ> icp;
  icp.setInputCloud(cloud_in);
  icp.setInputTarget(cloud_out);
  pcl::PointCloud<pcl::PointXYZ> Final;
  icp.align(Final);
  std::cout << "has converged:" << icp.hasConverged() << " score: " <<
  icp.getFitnessScore() << std::endl;
  std::cout << icp.getFinalTransformation() << std::endl;
}//end callback

int main (int argc, char** argv)
{
  ros::init(argc, argv, "pcl_icp");
  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe<PointCloud>("/points2", 1, callback);
  
  pub = nh.advertise<PointCloud> ("transform_points2", 1);
  ros::spin();
  return (0);
}
