#! /bin/sh
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu lucid main" > /etc/apt/sources.list.d/ros-latest.list'
wget http://packages.ros.org/ros.key -O - | sudo apt-key add -
sudo apt-get update
sudo apt-get install ros-fuerte-desktop-full ros-fuerte-laser-drivers ros-fuerte-joystick-drivers ros-fuerte-camera-drivers ros-fuerte-audio-common ros-fuerte-arm-navigation ros-fuerte-arm-navigation-experimental
mkdir -p ~/ros/dev_stacks
echo 'source /opt/ros/fuerte/setup.bash
export ROS_PACKAGE_PATH="~/ros/dev_stacks:$ROS_PACKAGE_PATH\"' >> ~/.bashrc
source ~/.bashrc
sudo apt-get install python-setuptools python-pip
sudo pip install -U rosinstall vcstools rosdep
cd ~/ros/dev_stacks
git clone git@github.com:evenator/cwru-ros-pkg.git
git remote add central git@github.com:cwru-robotics/cwru-ros-pkg.git
rosmake cwru_semi_stable
