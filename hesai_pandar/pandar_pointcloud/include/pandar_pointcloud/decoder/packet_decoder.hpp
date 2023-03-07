#pragma once

#include <pandar_msgs/msg/pandar_packet.hpp>
#include <fstream>
#include <vector>
#include "pandar_pointcloud/point_types.hpp"

namespace pandar_pointcloud
{
class PacketDecoder
{
public:
  virtual ~PacketDecoder(){};
  virtual void unpack(const pandar_msgs::msg::PandarPacket& raw_packet) = 0;

  // TODO: Remove this function
  // In Hesai's original driver, the decoder controls how many packets are used, but now the pandar_driver controls it.
  virtual bool hasScanned() = 0;

  virtual PointcloudXYZIRADT getPointcloud() = 0;
};
}  // namespace pandar_pointcloud