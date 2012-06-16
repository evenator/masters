#! /bin/sh
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu precise main" > /etc/apt/sources.list.d/ros-latest.list'
wget http://packages.ros.org/ros.key -O - | sudo apt-key add -
sudo apt-get update
sudo apt-get install ros-fuerte-desktop-full ros-fuerte-laser-drivers ros-fuerte-joystick-drivers ros-fuerte-camera-drivers
mkdir -p ~/ros/dev_stacks
echo 'source /opt/ros/fuerte/setup.bash
export ROS_PACKAGE_PATH="/home/abby/ros/dev_stacks:$ROS_PACKAGE_PATH\' >> ~/.bashrc
source ~/.bashrc
sudo apt-get install python-setuptools python-pip
sudo pip install -U rosinstall vcstools rosdep
cd ~/ros/dev_stacks
git clone git://github.com/esv/cwru-ros-pkg.git
rosmake --rosdep-install cwru_semi_stable