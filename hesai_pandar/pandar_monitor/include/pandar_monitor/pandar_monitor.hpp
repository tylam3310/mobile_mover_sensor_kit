/*
 * Copyright 2020 Tier IV, Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef PANDAR_MONITOR_PANDAR_MONITOR_H_
#define PANDAR_MONITOR_PANDAR_MONITOR_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "rclcpp/rclcpp.hpp"
#include "pandar_api/tcp_client.hpp"
#include "diagnostic_updater/diagnostic_updater.hpp"

namespace pandar_monitor{

class PandarMonitor : public rclcpp::Node
{
public:
  PandarMonitor(const rclcpp::NodeOptions & options);

protected:
  using DiagStatus = diagnostic_msgs::msg::DiagnosticStatus;

  void checkConnection(
    diagnostic_updater::DiagnosticStatusWrapper & stat);

  void checkTemperature(
    diagnostic_updater::DiagnosticStatusWrapper & stat);

  void checkPTP(
    diagnostic_updater::DiagnosticStatusWrapper & stat);

  diagnostic_updater::Updater updater_;
  std::unique_ptr<pandar_api::TCPClient> client_;
  bool diag_json_received_;

  std::string ip_address_;
  double timeout_;          
  float temp_cold_warn_;
  float temp_cold_error_;
  float temp_hot_warn_;
  float temp_hot_error_;
  float rpm_ratio_warn_;
  float rpm_ratio_error_;


  const std::map<int, const char *> rpm_dict_ = {
    {DiagStatus::OK, "OK"}, {DiagStatus::WARN, "RPM low"}, {DiagStatus::ERROR, "RPM too low"}};

  const char *position_[8] = {
    "Bottom circuit RT1",
    "Bottom circuit RT2",
    "Average temperature of the two laser emitting boards RT1 & RT2",
    "Laser emitting board RT1",
    "Laser emitting board RT2",
    "Receiving board RT1",
    "Top circuit RT1",
    "Top circuit RT2",
  };

  const char *ptp_[4] = {
    "free run",
    "tracking",
    "locked",
    "frozen",
  };
};

} // namespace pandar_monitor

#include <rclcpp_components/register_node_macro.hpp>
RCLCPP_COMPONENTS_REGISTER_NODE(pandar_monitor::PandarMonitor)

#endif  // PANDAR_MONITOR_PANDAR_MONITOR_H_
