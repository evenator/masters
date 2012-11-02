cd ~/ros/dev_stacks/
mkdir moveit
cd moveit
wget https://raw.github.com/ros-planning/moveit-docs/master/moveit.rosinstall
rosinstall . /opt/ros/fuerte moveit.rosinstall
. setup.bash
export ROS_PACKAGE_PATH="$HOME/ros/dev_stacks:$ROS_PACKAGE_PATH"
sudo apt-get install ros-fuerte-warehousewg ros-fuerte-control ros-fuerte-orocos-kinematics-dynamics ros-fuerte-robot-model ros-fuerte-pluginlib ros-fuerte-filters ros-fuerte-pcl ros-fuerte-dynamic-reconfigure ros-fuerte-python-qt-binding ros-fuerte-laser-pipeline ros-fuerte-image-common
cd ~/ros/dev_stacks/
mkdir rviz
cd rviz
svn co https://code.ros.org/svn/ros-pkg/stacks/visualization/trunk/ visualization
svn co https://code.ros.org/svn/ros-pkg/stacks/image_common/trunk/ image_common
sudo rosdep init
rosdep update
rosdep install rviz
rosmake moveit_rviz_plugin
rosmake pr2_arm_kinematics ompl_interface_ros planning_request_adapter_plugins

echo "Edit .bashrc and change the ROS lines to:
source /opt/ros/fuerte/setup.bash
source \$HOME/ros/dev_stacks/moveit/setup.bash
export ROS_PACKAGE_PATH=\"\$HOME/ros/dev_stacks:\$ROS_PACKAGE_PATH\""
