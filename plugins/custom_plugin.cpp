#ifndef _CUSTOM_PLUGIN_HH_
#define _CUSTOM_PLUGIN_HH_

#include <gazebo/common/Plugin.hh>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/common/common.hh>
#include <boost/shared_ptr.hpp>

namespace gazebo {
	typedef const boost::shared_ptr<const msgs::Any> Any;

	class CustomPlugin : public ModelPlugin {
			private: std::string namespace_;

			private: transport::NodePtr node;

			private: event::ConnectionPtr updateConnection;

			private: physics::ModelPtr model;
			private: physics::WorldPtr world;

			private: int countdown;
			private: int timer;

			private: transport::SubscriberPtr setTimer;
			private: transport::SubscriberPtr startAttack;

		public: CustomPlugin() {}
		public: virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf) {
			this->model = _model;
			this->world = _model->GetWorld();

			namespace_.clear();

			if (_sdf->HasElement("robotNamespace"))
				namespace_ = _sdf->GetElement("robotNamespace")->Get<std::string>();
			else
				gzerr << "[gazebo_custom_plugin] Please specify a robotNamespace.\n";


			this->node = transport::NodePtr(new transport::Node());
			this->node->Init(namespace_);

			this->timer = this->countdown = 18500; // Default timeout of 70 seconds

			// Listen to the update event. This event is broadcast every
      		// simulation iteration.
      		this->updateConnection = event::Events::ConnectWorldUpdateBegin(std::bind(&CustomPlugin::OnUpdate, this));

			// We can get parameters from the model/world file descriptor
			//if (_sdf->HasElement("time"))
			//	this->countdown = _sdf->GetElement("time")->Get<int>();
			
			std::string path = "/attack/";

			// Subscribe to the topic, and register a callback
			this->setTimer = this->node->Subscribe(path + "setTimer", &CustomPlugin::OnTimer, this);
			this->startAttack = this->node->Subscribe(path + "startAttack", &CustomPlugin::OnStartAttack, this);
		}

		public: virtual void OnUpdate() {
			//printf ("\n On updated custom plugin \n");
		}
		
		private: void OnTimer(Any &_msg) {
			if(_msg->type() == msgs::Any::STRING && _msg->has_string_value()) {
				printf ("got -> ");
				std::cout << _msg->string_value() << '\n';
			}
			printf ("\n Reset timer %d \n", this->countdown);
		}

		//private: void OnStartAttack(const std::string &_msg) {
		private: void OnStartAttack(Any &_msg) {
			if(_msg->type() == msgs::Any::INT32 && _msg->has_int_value()) {
				printf ("\n got %d", _msg->int_value());
			}
			printf ("\n Attacking the drone \n");
		}
	};
	// Tell Gazebo about this plugin, so that Gazebo can call Load on this plugin.
	GZ_REGISTER_MODEL_PLUGIN(CustomPlugin)
}
#endif