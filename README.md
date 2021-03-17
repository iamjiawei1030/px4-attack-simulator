# Attack Simulation

## Required packages

- python3-xmltodict
- meson

## Download and Build PX4/QGroundControl

[Download QGroundControl](https://docs.qgroundcontrol.com/master/en/getting_started/download_and_install.html#ubuntu)

[PX4](https://github.com/PX4/PX4-Autopilot)

```sh
git clone https://github.com/PX4/PX4-Autopilot.git --recursive
bash PX4-Autopilot/Tools/setup/ubuntu.sh # See note for Ubuntu 20.10

# Go to this project folder
SRC_DIR=/dir/to/PX4-Autopilot ./build.sh # Exit the simulator after successful build (Ctrl+C)

```

> For Ubuntu 20.10, modify Tools/setup/ubuntu.sh line 186 with `gazebo_version=`

## Generate a new world

```sh
mkdir worlds
SRC_DIR=/dir/to/PX4-Autopilot python3 scripts/modify_world.py
```

## Launch the Attack Center/Gazebo/QGroundControl

Double click `QGroundControl.AppImage` and run

```sh
SRC_DIR=/dir/to/PX4-Autopilot python3 scripts/app.py
```


## How to simulate an attack inside a gazebo plugin
- Copy or modify a plugin from `PX4-Autopilot/Tools/sitl_gazebo/src/gazebo_yyy_plugin.cpp`
```cpp
// Replace YYY with name of the plugin. This is required because the variables/functions
// will collide inside the namespace gazebo if they share the same name in multiple files.

// Declare an alias for the shared protobuf message
typedef const boost::shared_ptr<const msgs::Any> MessagePointer;

// Flag to simulate the attack
bool yyy_simulate_attack = false;
// bool yyy_simulate_attack_other = false;

// Signal listener
transport::SubscriberPtr yyy_listener;

// Add a signal handler
void OnYYYSignal(MessagePointer &_msg) {
	// We only handle integer signals, for different type of signals see the gazebo/msgs source code
	if(_msg->type() == msgs::Any::INT32 && _msg->has_int_value()) {
		int signal = _msg->int_value(); // Extract integer value from the protobuf message
		printf("[YYY] Got a signal %d", signal);

		// Handle different signals
		if(signal == 1) {
			// Enable flag to simulate attack
			yyy_simulate_attack = !yyy_simulate_attack; // Toggle the flag
		}
		// ...
		// signal == 2
		//     yyy_simulate_attack_other = true
	}
}

// Look for the Load function
void GazeboPlugin::Load(...) {
	...
	// At the end of the function

	// Assign a attack path to listen on
	std::string signal_path = "/attack/yyy"

	// Signal listener
	// Set the path to listen on and the handler function
	yyy_listener = node_handle_->Subscribe(signal_path, OnYYYSignal, this);
}

// Look for the Update function
void GazeboPlugin::OnUpdate(...) {
	if(yyy_simulate_attack) {
		// Simulate the attack
	// }
	// else if yyy_simulate_attack_other
	//		Simulate other attack
  	} else {
		// Do things normally
	}
}
```
- Rebuild the project and 
- Send signals with `./out/bin/command <signal_path> <signal>`