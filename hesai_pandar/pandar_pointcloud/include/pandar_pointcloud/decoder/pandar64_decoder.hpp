#pragma once

#include <array>
#include <rclcpp/rclcpp.hpp>
#include "pandar_pointcloud/calibration.hpp"
#include "packet_decoder.hpp"
#include "pandar64.hpp"

namespace pandar_pointcloud
{
  namespace pandar64
  {
    class Pandar64Decoder : public PacketDecoder
    {
    public:
      enum class ReturnMode : int8_t
      {
        DUAL,
        STRONGEST,
        LAST,
      };
      enum ReturnType : uint8_t
      {
        INVALID = 0,
        SINGLE_STRONGEST,
        SINGLE_LAST,
        DUAL_FIRST,
        DUAL_LAST,
        DUAL_ONLY,
      };

      explicit Pandar64Decoder(rclcpp::Node & node,
                               Calibration& calibration,
                               float scan_phase = 0.0f,
                               double dual_return_distance_threshold = 0.1,
                               ReturnMode return_mode = ReturnMode::DUAL);

      void unpack(const pandar_msgs::msg::PandarPacket& raw_packet) override;

      PointXYZIRADT build_point(int block_id, int unit_id, uint8_t return_type);

      bool hasScanned() override;

      PointcloudXYZIRADT getPointcloud() override;

    private:
      rclcpp::Logger logger_;
      rclcpp::Clock::SharedPtr clock_;

      bool parsePacket(const pandar_msgs::msg::PandarPacket& raw_packet);

      PointcloudXYZIRADT convert(int block_id);

      PointcloudXYZIRADT convert_dual(int block_id);

      std::array<float, UNIT_NUM> elev_angle_{};
      std::array<float, UNIT_NUM> azimuth_offset_{};

      std::array<float, UNIT_NUM> firing_offset_{};
      std::array<float, BLOCK_NUM> block_offset_single_{};
      std::array<float, BLOCK_NUM> block_offset_dual_{};

      ReturnMode return_mode_;
      double dual_return_distance_threshold_;
      Packet packet_{};

      PointcloudXYZIRADT scan_pc_;
      PointcloudXYZIRADT overflow_pc_;

      uint16_t scan_phase_;
      int last_phase_;
      bool has_scanned_;
    };

  }  // namespace pandar_qt
}  // namespace pandar_pointcloud
