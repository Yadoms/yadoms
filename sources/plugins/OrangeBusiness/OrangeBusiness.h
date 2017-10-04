#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include <shared/event/EventTimer.h>
#include "OrangeBusinessConfiguration.h"
#include "urlManager.h"
#include "Decoder.h"
#include "deviceManager.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This class is the IPX800 plugin
//--------------------------------------------------------------
class COrangeBusiness : public plugin_cpp_api::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   COrangeBusiness();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~COrangeBusiness();

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

   //--------------------------------------------------------------
   /// \brief Update the configuration of the plugin after a change
   /// \param[in] api                   pointer to the API
   /// \param[in] neMessage             The new message to be processed
   //--------------------------------------------------------------
   void registerAllDevices(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief Update the configuration of the plugin after a change
   /// \param[in] api                   pointer to the API
   /// \param[in] neMessage             The new message to be processed
   //--------------------------------------------------------------
   void registerActivatedDevices(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief Update the configuration of the plugin after a change
   /// \param[in] api                   pointer to the API
   /// \param[in] newConfigurationData  The new configuration of the module
   //--------------------------------------------------------------
   void onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                              const shared::CDataContainer& newConfigurationData);

private:

   //--------------------------------------------------------------
   /// \brief Configuration of the device
   //--------------------------------------------------------------
   COrangeBusinessConfiguration m_configuration;

   //--------------------------------------------------------------
   /// \brief	Wait for answer timer
   //--------------------------------------------------------------
   boost::shared_ptr<shared::event::CEventTimer> m_waitForAnswerTimer;

   urlManager m_frameManager;

   boost::shared_ptr<CEquipmentManager> m_equipmentManager;

   boost::shared_ptr<CDecoder> m_decoder;

   //--------------------------------------------------------------
   /// \brief developer mode for logs
   //--------------------------------------------------------------
   bool m_isDeveloperMode;
};