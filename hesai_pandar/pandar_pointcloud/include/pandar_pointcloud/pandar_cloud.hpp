#pragma once

#include <rclcpp/rclcpp.hpp>
#include <pandar_msgs/msg/pandar_scan.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <pandar_api/tcp_client.hpp>
#include "pandar_pointcloud/calibration.hpp"
#include "pandar_pointcloud/decoder/packet_decoder.hpp"
// #include "pandar_pointcloud/tcp_command_client.hpp"


#include <string>

namespace pandar_pointcloud
{
class PandarCloud : public rclcpp::Node
{
public:
  PandarCloud(const rclcpp::NodeOptions & options);
  ~PandarCloud();

private:
  bool setupCalibration();
  void onProcessScan(const pandar_msgs::msg::PandarScan::SharedPtr msg);
  pcl::PointCloud<PointXYZIR>::Ptr convertPointcloud(const pcl::PointCloud<PointXYZIRADT>::ConstPtr& input_pointcloud);

  std::string model_;
  std::string return_mode_;
  std::string device_ip_;
  std::string calibration_path_;
  double dual_return_distance_threshold_;
  double scan_phase_;
  std::vector<double> angle_range_;
  std::vector<double> distance_range_;

  rclcpp::Subscription<pandar_msgs::msg::PandarScan>::SharedPtr pandar_packet_sub_;
  rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr pandar_points_pub_;
  rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr pandar_points_ex_pub_;

  std::shared_ptr<PacketDecoder> decoder_;
  std::shared_ptr<pandar_api::TCPClient> tcp_client_;
  Calibration calibration_;
};

}  // namespace pandar_pointcloud

#include <rclcpp_components/register_node_macro.hpp>

RCLCPP_COMPONENTS_REGISTER_NODE(pandar_pointcloud::PandarCloud)