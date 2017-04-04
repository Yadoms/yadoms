#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "WESConfiguration.h"
#include "IOManager.h"
#include "WESFactory.h"
#include <shared/event/EventTimer.h>

#include "equipments/IEquipment.h"
#include "pluginStateMachine/PluginStateMachine.hpp"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This class is the WES plugin
//--------------------------------------------------------------
class CWES : public plugin_cpp_api::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CWES();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CWES();

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

private:

	//--------------------------------------------------------------
	/// \brief Initialize the communication with the WES
	/// \param[in] api                   pointer to the API
	//--------------------------------------------------------------
	void initWES(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief Name of the plugin
   //--------------------------------------------------------------
   std::string m_deviceName;

   //--------------------------------------------------------------
   /// \brief Configuration of the device
   //--------------------------------------------------------------
   CWESConfiguration m_configuration;

   //--------------------------------------------------------------
   /// \brief The IO Manager
   //--------------------------------------------------------------
   boost::shared_ptr<equipments::IEquipment> m_deviceManager;

   //--------------------------------------------------------------
   /// \brief The factory
   //--------------------------------------------------------------
   //boost::shared_ptr<CWESFactory> m_factory;

   //--------------------------------------------------------------
   /// \brief	Refresh timer
   //--------------------------------------------------------------
   //boost::shared_ptr<shared::event::CEventTimer> m_refreshTimer;

   //--------------------------------------------------------------
   /// \brief	PluginState
   //--------------------------------------------------------------
   msm::back::state_machine<pluginStateMachine> m_pluginState;
};