# Attack Simulation

## Required packages

- python3-xmltodict
- meson

## Download and Build PX4/QGroundControl

[Download QGroundControl](https://docs.qgroundcontrol.com/master/en/getting_started/download_and_install.html#ubuntu)

[PX4](https://github.com/PX4/PX4-Autopilot)

```
git clone https://github.com/PX4/PX4-Autopilot.git --recursive
bash PX4-Autopilot/Tools/setup/ubuntu.sh # See note for Ubuntu 20.10

# Go to this project folder
SRC_DIR=/dir/to/PX4-Autopilot ./build.sh # Exit the simulator after successful build (Ctrl+C)

```

> For Ubuntu 20.10, modify Tools/setup/ubuntu.sh line 186 with `gazebo_version=`

## Generate a new world

```
mkdir worlds
SRC_DIR=/dir/to/PX4-Autopilot python3 scripts/modify_world.py
```

## Launch the Attack Center/Gazebo/QGroundControl

Double click `QGroundControl.AppImage` and run

```
SRC_DIR=/dir/to/PX4-Autopilot python3 scripts/app.py
```
