#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "ZWaveConfiguration.h"
#include "IZWaveController.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This class is the ZWave plugin entry point
//--------------------------------------------------------------
class CZWave : public plugin_cpp_api::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CZWave();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CZWave();

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

private:
   //--------------------------------------------------------------
   /// \brief	Stop the controller
   //--------------------------------------------------------------
   void stopController() const;

   //--------------------------------------------------------------
   /// \brief	The main loop (after init success)
   /// \param [in] api The plugin API
   //--------------------------------------------------------------
   void mainLoop(const boost::shared_ptr<yApi::IYPluginApi>& api);

   //--------------------------------------------------------------
   /// \brief	Handle device commands
   /// \param [in] api The plugin API
   //--------------------------------------------------------------
   void onEventDeviceCommand(const boost::shared_ptr<yApi::IYPluginApi>& api) const;

   //--------------------------------------------------------------
   /// \brief	Handle device configuration schema request
   /// \param [in] api The plugin API
   //--------------------------------------------------------------
   void onDeviceConfigurationSchemaRequest(const boost::shared_ptr<yApi::IYPluginApi>& api) const;

   //--------------------------------------------------------------
   /// \brief	Handle updateing device configuration
   /// \param [in] api The plugin API
   //--------------------------------------------------------------
   void setDeviceConfiguration(const boost::shared_ptr<yApi::IYPluginApi>& api) const;

   //--------------------------------------------------------------
   /// \brief	Handle extra quries
   /// \param [in] api The plugin API
   //--------------------------------------------------------------
   void onExtraQuery(const boost::shared_ptr<yApi::IYPluginApi>& api) const;  
   
   //--------------------------------------------------------------
   /// \brief	Handle plugin configuration update
   /// \param [in] api The plugin API
   //--------------------------------------------------------------
   void onEventUpdateConfiguration(const boost::shared_ptr<yApi::IYPluginApi>& api);

   //--------------------------------------------------------------
   /// \brief	Handle device declaration
   /// \param [in] api The plugin API
   //--------------------------------------------------------------
   static void onDeclareDevice(const boost::shared_ptr<yApi::IYPluginApi>& api);

   //--------------------------------------------------------------
   /// \brief	Handle device information update
   /// \param [in] api The plugin API
   //--------------------------------------------------------------
   static void onUpdateDeviceInfo(const boost::shared_ptr<yApi::IYPluginApi>& api);

   //--------------------------------------------------------------
   /// \brief	Handle device state update
   /// \param [in] api The plugin API
   //--------------------------------------------------------------
   static void onUpdateDeviceState(const boost::shared_ptr<yApi::IYPluginApi>& api);

   //--------------------------------------------------------------
   /// \brief	Handle keyword update
   /// \param [in] api The plugin API
   //--------------------------------------------------------------
   static void onUpdateKeyword(const boost::shared_ptr<yApi::IYPluginApi>& api);

   //--------------------------------------------------------------
   /// \brief	Handle internal configuration update. This case is used, when the device itself send a new configuration value (maybe after reset,...)
   /// \param [in] api The plugin API
   //--------------------------------------------------------------
   void onUpdateConfiguration(const boost::shared_ptr<yApi::IYPluginApi>& api) const;

   //--------------------------------------------------------------
   /// \brief	Handle internal state change
   /// \param [in] api The plugin API
   //--------------------------------------------------------------
   void onInternalStateChange(const boost::shared_ptr<yApi::IYPluginApi>& api) const;


   //--------------------------------------------------------------
   /// \brief	Check if a version upgrade have been done
   /// \param [in] api The plugin API
   //--------------------------------------------------------------
   void checkVersionUpgrade(const boost::shared_ptr<yApi::IYPluginApi>& api) const;

   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   CZWaveConfiguration m_configuration;

   //--------------------------------------------------------------
   /// \brief	The ZWave controller
   //--------------------------------------------------------------
   boost::shared_ptr<IZWaveController> m_controller;

public:

   enum
   {
      kDeclareDevice = yApi::IYPluginApi::kPluginFirstEventId,
      kUpdateDeviceInfo,
      kUpdateDeviceState,
      kUpdateKeyword,
      kUpdateConfiguration,
      kInternalStateChange
   };
   
};



