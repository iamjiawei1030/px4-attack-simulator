#include <gazebo/gazebo_config.h>
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/gazebo_client.hh>

int main(int _argc, char **_argv) {

	if (_argc < 3) {
		std::cout << "Need 2 arguments: <topic_name> <signal>" << std::endl;
		return 1;
	}
	
	std::string topic_name(_argv[1]);
	int signal = std::atoi(_argv[2]);

	gazebo::client::setup(_argc, _argv);

	// Create our node for communication
	gazebo::transport::NodePtr node(new gazebo::transport::Node());
	node->Init();

	// Publish to the custom topic
	gazebo::transport::PublisherPtr publisher = node->Advertise<gazebo::msgs::Any>(topic_name);

	// Wait for a subscriber to connect to this publisher
	publisher->WaitForConnection();

	// Create a pb message
	gazebo::msgs::Any integer = gazebo::msgs::ConvertAny(signal);

	// Send the message
	publisher->Publish(integer);
	
	// Make sure to shut everything down.
	gazebo::client::shutdown();
}