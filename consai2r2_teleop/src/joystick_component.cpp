
#include "consai2r2_teleop/joystick_component.hpp"

#include <chrono>
#include <cmath>
#include <cstdio>
#include <memory>
#include <string>

using namespace std::chrono_literals;

namespace joystick {
JoystickComponent::JoystickComponent(const rclcpp::NodeOptions &options)
    : Node("consai2r2_teleop", options) {
  /**
   * パラメータの宣言
   */
  this->declare_parameter("button_shutdown_1", 8);
  this->declare_parameter("button_shutdown_2", 8);
  this->declare_parameter("button_move_enable", 4);
  this->declare_parameter("axis_vel_surge", 1);
  this->declare_parameter("axis_vel_sway", 0);
  this->declare_parameter("axis_vel_angular", 2);

  this->declare_parameter("button_kick_enable");
  this->declare_parameter("button_kick_straight");
  this->declare_parameter("button_kick_chip");
  this->declare_parameter("axis_kick_power");

  this->declare_parameter("button_dribble_enable");
  this->declare_parameter("axis_dribble_power");

  this->declare_parameter("button_id_enable");
  this->declare_parameter("axis_id_change");

  this->declare_parameter("button_color_enable");
  this->declare_parameter("axis_color_change");

  this->declare_parameter("button_all_id_1");
  this->declare_parameter("button_all_id_2");
  this->declare_parameter("button_all_id_3");
  this->declare_parameter("button_all_id_4");

  this->declare_parameter("button_path_enable");
  this->declare_parameter("button_add_pose");
  this->declare_parameter("button_delete_path");
  this->declare_parameter("button_send_target");

  /**
   * パラメータの代入
   */
  this->get_parameter("button_shutdown_1", button_conf_.shutdown_1);
  this->get_parameter("button_shutdown_2", button_conf_.shutdown_2);
  this->get_parameter("button_move_enable", button_conf_.move_enable);
  this->get_parameter("axis_vel_surge", axis_conf_.surge_vel);
  this->get_parameter("axis_vel_sway", axis_conf_.sway_vel);
  this->get_parameter("axis_vel_angular", axis_conf_.angular_vel);

  this->get_parameter("button_kick_enable", button_conf_.kick_enable);
  this->get_parameter("button_kick_straight", button_conf_.kick_straight);
  this->get_parameter("button_kick_chip", button_conf_.kick_chip);
  this->get_parameter("axis_kick_power", axis_conf_.kick_power);

  this->get_parameter("button_dribble_enable", button_conf_.dribble_enable);
  this->get_parameter("axis_dribble_power", axis_conf_.dribble_power);

  this->get_parameter("button_id_enable", button_conf_.id_enable);
  this->get_parameter("axis_id_change", axis_conf_.id_change);

  this->get_parameter("button_color_enable", button_conf_.color_enable);
  this->get_parameter("axis_color_change", axis_conf_.color_change);

  this->get_parameter("button_all_id_1", button_conf_.all_id_1);
  this->get_parameter("button_all_id_2", button_conf_.all_id_2);
  this->get_parameter("button_all_id_3", button_conf_.all_id_3);
  this->get_parameter("button_all_id_4", button_conf_.all_id_4);

  this->get_parameter("button_path_enable", button_conf_.path_enable);
  this->get_parameter("button_add_pose", button_conf_.add_pose);
  this->get_parameter("button_delete_path", button_conf_.delete_path);
  this->get_parameter("button_send_target", button_conf_.send_target);

  auto callback = [this](const sensor_msgs::msg::Joy::SharedPtr msg) -> void {
    publish_robot_commands(msg);
  };

  pub_commands_ = create_publisher<consai2r2_msgs::msg::RobotCommands>(
      "robot_commands", 10);
  sub_joy_ = create_subscription<sensor_msgs::msg::Joy>("joy", 10, callback);
}

void JoystickComponent::publish_robot_commands(
    const sensor_msgs::msg::Joy::SharedPtr msg) {
  // TODO: WE HAVE TO USE ROS_PARAM
  const int BUTTON_SHUTDOWN_1 = 8;
  const int BUTTON_SHUTDOWN_2 = 8;
  const int BUTTON_MOVE_ENABLE = 4;
  const int AXIS_VEL_SURGE = 1;
  const int AXIS_VEL_SWAY = 0;
  const int AXIS_VEL_ANGULAR = 2;

  const double MAX_VEL_SURGE = 1.0;
  const double MAX_VEL_SWAY = 1.0;
  const double MAX_VEL_ANGULAR = M_PI;

  consai2r2_msgs::msg::RobotCommand command;

  //シャットダウン
  if (msg->buttons[button_conf_.shutdown_1] &&
      msg->buttons[button_conf_.shutdown_2]) {
    rclcpp::shutdown();
    return;
  }

  if (msg->buttons[BUTTON_MOVE_ENABLE]) {
    command.vel_surge = msg->axes[AXIS_VEL_SURGE] * MAX_VEL_SURGE;
    command.vel_sway = msg->axes[AXIS_VEL_SWAY] * MAX_VEL_SWAY;
    command.vel_angular = msg->axes[AXIS_VEL_ANGULAR] * MAX_VEL_ANGULAR;
  }

  command.robot_id = 0;

  consai2r2_msgs::msg::RobotCommands robot_commands;
  robot_commands.header.stamp = this->now();
  robot_commands.is_yellow = false;
  robot_commands.commands.push_back(command);

  pub_commands_->publish(robot_commands);
}

} // namespace joystick

#include <rclcpp_components/register_node_macro.hpp>

RCLCPP_COMPONENTS_REGISTER_NODE(joystick::JoystickComponent)
