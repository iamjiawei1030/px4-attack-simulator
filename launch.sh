#!/usr/bin/env bash

set -e

if [[ -z "$SRC_DIR" ]]; then
    echo "You need to specify the absolute path of PX4-Autopilot with SRC_DIR"
	exit 1
fi

BUILD_DIR="$SRC_DIR/build/px4_sitl_default"

world="empty"
program="gazebo"
model="iris"
sitl_bin="$BUILD_DIR/bin/px4"
plugin_path="$BUILD_DIR/build_gazebo"
model_path="$SRC_DIR/Tools/sitl_gazebo/models"
world_path="$SRC_DIR/Tools/sitl_gazebo/worlds"
test_data="$SRC_DIR/test_data"
etc_path="$BUILD_DIR/etc"
rootfs="$BUILD_DIR/tmp/rootfs"
pwd=$(pwd)

SIM_PID=0

echo SITL ARGS
echo sitl_bin: $sitl_bin
echo program: $program
echo model: $model
echo world: $world
echo plugin_path: $plugin_path

# kill process names that might stil
# be running from last time
pkill -x gazebo || true
pkill -x gzclient || true
pkill -x gzserver || true
pkill -x px4 || true
pkill -x px4-simulator || true

# Set the plugin path so Gazebo finds our model and sim
export PX4_SIM_MODEL=${model}
export GAZEBO_PLUGIN_PATH=${plugin_path}:${pwd}/out/plugins/
export GAZEBO_MODEL_PATH=${model_path}

#Spawn predefined world
if [[ -z "$world" ]]; then
	world_path="$world_path/empty.world"
else
    world_path="$world_path/${world}.world"
fi

echo gzserver startup
gzserver $world_path &
SIM_PID=$!

while gz model --verbose --spawn-file="${pwd}/worlds/iris.xml" --model-name=${model} -x 1.01 -y 0.98 -z 0.83 2>&1 | grep -q "An instance of Gazebo is not running."; do
	echo "gzserver not ready yet, trying again!"
	sleep 1
done

# gzserver needs to be running to avoid a race. Since the launch
# is putting it into the background we need to avoid it by backing off
sleep 3
nice -n 20 gzclient --verbose --gui-client-plugin libgazebo_user_camera_plugin.so &
GUI_PID=$!

pushd "$rootfs" >/dev/null

# Do not exit on failure now from here on because we want the complete cleanup
set +e

sitl_command="${sitl_bin} -d ${etc_path} -s etc/init.d-posix/rcS -t ${test_data}"

echo SITL COMMAND: $sitl_command
eval $sitl_command

popd >/dev/null

kill -9 $SIM_PID
kill -9 $GUI_PID