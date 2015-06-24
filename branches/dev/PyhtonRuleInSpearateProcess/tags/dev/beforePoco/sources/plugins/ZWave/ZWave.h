#pragma once
#include <shared/plugin/IPlugin.h>
#include "ZWaveConfiguration.h"

#include "IZWaveController.h"

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

//--------------------------------------------------------------
/// \brief	This class is the ZWave plugin entry point
//--------------------------------------------------------------
class CZWave : public shared::plugin::IPlugin
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
   virtual void doWork(boost::shared_ptr<yApi::IYadomsApi> context);
   // [END] IPlugin implementation

private:
   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   CZWaveConfiguration m_configuration;

   //--------------------------------------------------------------
   /// \brief	The ZWave controller
   //--------------------------------------------------------------
   boost::shared_ptr<IZWaveController> m_controller;
};



