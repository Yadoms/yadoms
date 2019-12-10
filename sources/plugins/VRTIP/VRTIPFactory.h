#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/event/EventHandler.hpp>
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationRequest.h>

#include "equipments/IEquipment.h"
#include "IOManager.h"
// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Class where all Keywords are created and updated
/// \note   
//--------------------------------------------------------------
class CVRTIPFactory
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
	CVRTIPFactory();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CVRTIPFactory();

   //--------------------------------------------------------------
   /// \brief	    create all the configuration (all devices, ...)
   /// \param[in] api                  yPluginApi API
   /// \param[in] configuration        configuration of the plugin
   //--------------------------------------------------------------
   boost::shared_ptr<CIOManager> loadConfiguration(boost::shared_ptr<yApi::IYPluginApi> api) const;

   //--------------------------------------------------------------
   /// \brief	    create all the configuration (all devices, ...)
   /// \param[in] api                  yPluginApi API
   /// \param[in] device               the device name
   /// \return                         The IO Manager containing all parameters
   //--------------------------------------------------------------
   boost::shared_ptr<equipments::IEquipment> createEquipment(boost::shared_ptr<yApi::IYPluginApi> api,
                                                             const std::string& device);

   //--------------------------------------------------------------
   /// \brief	    createDeviceManually
   /// \param[in] api                  yPluginApi API
   /// \param[in] data                 information for the creation of the new device
   //--------------------------------------------------------------
   std::string createDeviceManually(boost::shared_ptr<yApi::IYPluginApi> api,
	                                boost::shared_ptr<CIOManager> ioManager,
                                    const yApi::IManuallyDeviceCreationData& data) const;

private:
   
};