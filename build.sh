#!/usr/bin/env bash

set -e

if [[ -z "$SRC_DIR" ]]; then
    echo "You need to specify the absolute path of PX4-Autopilot with SRC_DIR"
	exit 1
fi

rm -rf out
mkdir -p out

meson build
ninja -C build
DESTDIR=$(pwd)/out ninja -C build install

PLUGINS="gazebo_gps_plugin.cpp gazebo_imu_plugin.cpp"
for PLUGIN in $PLUGINS
do
    cp ./plugins/$PLUGIN $SRC_DIR/Tools/sitl_gazebo/src/$PLUGIN
done

pushd "$SRC_DIR" >/dev/null
make px4_sitl gazebo
popd >/dev/null