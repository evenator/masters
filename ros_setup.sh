#! /bin/sh

#Install ROS
sudo sh -c 'echo "deb http://http://mobilerobots.case.edu/mirror/packages.ros.org/ros/ubuntu precise main" > /etc/apt/sources.list.d/ros-latest.list'
wget http://packages.ros.org/ros.key -O - | sudo apt-key add -
sudo apt-get update
sudo apt-get install -y ros-fuerte-desktop-full ros-fuerte-laser-drivers ros-fuerte-joystick-drivers ros-fuerte-camera-drivers ros-fuerte-audio-common ros-fuerte-arm-navigation ros-fuerte-arm-navigation-experimental ros-fuerte-octomap ros-fuerte-octomap-mapping ros-fuerte-openni-camera ros-fuerte-openni-launch ros-fuerte-slam-gmapping ros-fuerte-pr2_object_manipulation python-scipy
mkdir -p ~/ros/dev_stacks
echo 'source /opt/ros/fuerte/setup.bash
export ROS_PACKAGE_PATH="$HOME/ros/dev_stacks:$ROS_PACKAGE_PATH"' >> ~/.bashrc

#Source the new .bashrc
source ~/.bashrc

#Install rosinstall and rosdep, cuz they're handy
sudo apt-get install python-setuptools python-pip
sudo pip install -U rosinstall vcstools rosdep

#Make dev stacks
mkdir -p ~/ros/dev_stacks
cd ~/ros/dev_stacks

#CWRU ROS Package
git clone https://github.com/evenator/cwru-ros-pkg.git
cd cwru-ros-pkg
git remote add central https://github.com/cwru-robotics/cwru-ros-pkg.git
rosmake cwru_semi_stable
cd ..

#Modified ROS Serial (Supports services. May not be necessary once groovy is out)
git clone https://github.com/evenator/rosserial.git
rosmake rosserial_python
rosmake rosserial_msgs

#Modified ROS Industrial (Has hard-coded joints and description for IRB-120. May not be necessary once groovy is out)
git clone https://github.com/evenator/swri-ros-pkg2.git swri-ros-pkg
rosmake industrial_core industrial_experimental industrial_msgs abb_common

cd /opt/ros/fuerte/stacks/arm_navigation/
sudo chown -R $USER:$USER arm_kinematics_constraint_aware/
cd arm_kinematics_constraint_aware/
sudo wget https://code.ros.org/trac/ros-pkg/raw-attachment/ticket/5586/fk_solver.patch
sudo patch -p1 < fk_solver.patch
rm ROS_NOBUILD
rosmake arm_kinematics_constraint_aware
