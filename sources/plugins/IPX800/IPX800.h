#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "IPX800Configuration.h"
#include "IOManager.h"
#include "IPX800Factory.h"
#include <shared/event/EventTimer.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This class is the IPX800 plugin
//--------------------------------------------------------------
class CIPX800 : public plugin_cpp_api::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CIPX800();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CIPX800();

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

   //--------------------------------------------------------------
   /// \brief Update the configuration of the plugin after a change
   /// \param[in] api                   pointer to the API
   /// \param[in] newConfigurationData  The new configuration of the module
   //--------------------------------------------------------------
   void onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                              const shared::CDataContainer& newConfigurationData);

private:

   //--------------------------------------------------------------
   /// \brief Initialize the communication with the IPX800
   /// \param[in] api                   pointer to the API
   //--------------------------------------------------------------
   void initIPX800(boost::shared_ptr<yApi::IYPluginApi> api) const;

   //--------------------------------------------------------------
   /// \brief Name of the plugin
   //--------------------------------------------------------------
   std::string m_deviceName;

   //--------------------------------------------------------------
   /// \brief Configuration of the device
   //--------------------------------------------------------------
   CIPX800Configuration m_configuration;

   //--------------------------------------------------------------
   /// \brief The IO Manager
   //--------------------------------------------------------------
   boost::shared_ptr<CIOManager> m_ioManager;

   //--------------------------------------------------------------
   /// \brief The factory
   //--------------------------------------------------------------
   boost::shared_ptr<CIPX800Factory> m_factory;

   //--------------------------------------------------------------
   /// \brief	Refresh timer
   //--------------------------------------------------------------
   boost::shared_ptr<shared::event::CEventTimer> m_refreshTimer;
};

