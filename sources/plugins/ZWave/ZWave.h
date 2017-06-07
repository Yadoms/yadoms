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
   void StopController() const;

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
      kUpdateKeyword,
      kUpdateConfiguration,
      kInternalStateChange
   };
   
};



