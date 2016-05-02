#!/bin/sh

#!/bin/bash

curdir=$(cd `dirname $0` && pwd)

sudo apt-get update
sudo apt-get upgrade

sudo apt-get install build-essential

sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev

sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev

sudo apt-get automake
sudo apt-get autoconf
sudo apt-get autopoint 
sudo apt-get gettext  
sudo apt-get libtool  

cd libgphoto2-2.5.10

sudo ./configure

sudo make

sudo make install all

sudo cd ..

sudo -S apt-get -y install libopencv-dev build-essential cmake git libgtk2.0-dev pkg-config python-dev python-numpy libdc1394-22 libdc1394-22-dev libjpeg-dev libpng12-dev libtiff4-dev libjasper-dev libavcodec-dev libavformat-dev libswscale-dev libxine-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev libv4l-dev libtbb-dev libqt4-dev libfaac-dev libmp3lame-dev libopencore-amrnb-dev libopencore-amrwb-dev libtheora-dev libvorbis-dev libxvidcore-dev x264 v4l-utils unzip
 
FOLDER_NAME="opencv"

mkdir ${FOLDER_NAME}

cd ${FOLDER_NAME}

wget https://github.com/Itseez/opencv/archive/3.0.0-alpha.zip -O opencv-3.0.0-alpha.zip
 
unzip opencv-3.0.0-alpha.zip
 
cd opencv-3.0.0-alpha
 
mkdir build
 
cd build

cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_TBB=ON -D WITH_V4L=ON -D WITH_QT=ON -D WITH_OPENGL=ON ..

make -j $(nproc)
 
sudo make install
 
sudo -S /bin/bash -c 'echo "/usr/local/lib" > /etc/ld.so.conf.d/opencv.conf'

sudo -S ldconfig

sudo -S apt-get install gphoto2

sudo -S libgphoto2-6-dev




