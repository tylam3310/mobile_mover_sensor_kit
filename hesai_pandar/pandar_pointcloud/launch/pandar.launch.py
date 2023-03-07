import launch
from launch.actions import DeclareLaunchArgument
from launch.actions import OpaqueFunction
from launch.actions import SetLaunchConfiguration
from launch.conditions import IfCondition
from launch.conditions import UnlessCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import ComposableNodeContainer
from launch_ros.actions import LoadComposableNodes
from launch_ros.descriptions import ComposableNode
import yaml

def launch_setup(context, *args, **kwargs):
    def create_parameter_dict(*args):
        result = {}
        for x in args:
            result[x] = LaunchConfiguration(x)
        return result

    nodes = []

    nodes.append(
        ComposableNode(
            package="pandar_pointcloud",
            plugin="pandar_pointcloud::PandarCloud",
            name="pandar_cloud",
            parameters=[{**create_parameter_dict('scan_phase', 'model', 'device_ip', 'calibration'),
        }],
        remappings=[('pandar_points', 'pointcloud_raw'),
                    ('pandar_points_ex', 'pointcloud_raw_ex')],
        extra_arguments=[{
            'use_intra_process_comms': LaunchConfiguration('use_intra_process')
        }],
    )
    )

    container = ComposableNodeContainer(
        name='pandar_node_container',
        namespace='pointcloud_preprocessor',
        package='rclcpp_components',
        executable=LaunchConfiguration('container_executable'),
        composable_node_descriptions=nodes,
    )

    driver_component = ComposableNode(
        package='pandar_driver',
        plugin='pandar_driver::PandarDriver',
        name='pandar_driver',
        parameters=[{**create_parameter_dict('pcap', 'device_ip', 'lidar_port', 'gps_port', 'scan_phase', 'model', 'frame_id'),
                     }],
    )

    loader = LoadComposableNodes(
        composable_node_descriptions=[driver_component],
        target_container=container,
        condition=launch.conditions.IfCondition(LaunchConfiguration('launch_driver')),
    )

    return [container, loader]


def generate_launch_description():
    launch_arguments = []

    def add_launch_arg(name: str, default_value=None):
        launch_arguments.append(DeclareLaunchArgument(name, default_value=default_value))


    add_launch_arg('launch_driver', 'True')
    add_launch_arg('pcap', '')
    add_launch_arg('device_ip', '192.168.1.201')
    add_launch_arg('lidar_port', '2368')
    add_launch_arg('gps_port', '10110')
    add_launch_arg('scan_phase', '0.0')
    add_launch_arg('model', 'PandarQT')
    add_launch_arg('frame_id', 'pandar')
    add_launch_arg('calibration', '')

    add_launch_arg('container_name', 'pandar_composable_node_container')
    add_launch_arg('use_multithread', 'False')
    add_launch_arg('use_intra_process', 'False')

    set_container_executable = SetLaunchConfiguration(
        'container_executable',
        'component_container',
        condition=UnlessCondition(LaunchConfiguration('use_multithread'))
    )

    set_container_mt_executable = SetLaunchConfiguration(
        'container_executable',
        'component_container_mt',
        condition=IfCondition(LaunchConfiguration('use_multithread'))
    )

    return launch.LaunchDescription(launch_arguments +
                                    [set_container_executable,
                                     set_container_mt_executable] +
                                    [OpaqueFunction(function=launch_setup)])
