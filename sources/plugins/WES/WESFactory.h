#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/event/EventHandler.hpp>
#include "IWESConfiguration.h"
#include "IOManager.h"
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationRequest.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Class where all Keywords are created and updated
/// \note   
//--------------------------------------------------------------
class CWESFactory
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   CWESFactory();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CWESFactory();

   //--------------------------------------------------------------
   /// \brief	    createDeviceManually
   /// \param[in] api                  yPluginApi API
   /// \param[in] configuration        configuration of the plugin
   //--------------------------------------------------------------
   boost::shared_ptr<CIOManager> loadConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                 const boost::shared_ptr<IWESConfiguration> configuration);

   //--------------------------------------------------------------
   /// \brief	    createDeviceManually
   /// \param[in] api                  yPluginApi API
   /// \param[in] data                 information for the creation of the new device
   //--------------------------------------------------------------
   std::string createDeviceManually(boost::shared_ptr<yApi::IYPluginApi> api,
                                    const boost::shared_ptr<CIOManager> iomanager,
                                    const yApi::IManuallyDeviceCreationData& data,
                                    const boost::shared_ptr<IWESConfiguration> configuration);

   //--------------------------------------------------------------
   /// \brief	                     Process a command received from Yadoms
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] deviceRemoved    The name of the device removed
   //--------------------------------------------------------------
   void removeDevice(boost::shared_ptr<yApi::IYPluginApi> api, std::string deviceRemoved);

private:
   
};