#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IIPX800Configuration.h"
#include "IOManager.h"
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationRequest.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Class where all Keywords are created and updated
/// \note   
//--------------------------------------------------------------
class CIPX800Factory
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] api                 yPluginApi API
   /// \param[in] device              The device name
   /// \param[in] configuration       The plugin configuration
   //--------------------------------------------------------------
   CIPX800Factory(boost::shared_ptr<yApi::IYPluginApi> api,
                  const std::string& device,
                  const IIPX800Configuration& configuration);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CIPX800Factory();

   //--------------------------------------------------------------
   /// \brief	    bindSlotsX8R
   /// \return     slots available for X8R
   //--------------------------------------------------------------
   shared::CDataContainerSharedPtr bindSlotsX8R();

   //--------------------------------------------------------------
   /// \brief	    bindSlotsX8D
   /// \return     slots available for X8D
   //--------------------------------------------------------------
   shared::CDataContainerSharedPtr bindSlotsX8D();

   //--------------------------------------------------------------
   /// \brief	    bindSlotsX24D
   /// \return     slots available for X8D
   //--------------------------------------------------------------
   shared::CDataContainerSharedPtr bindSlotsX24D();

   //--------------------------------------------------------------
   /// \brief	    createDeviceManually
   /// \param[in] api                  yPluginApi API
   /// \param[in] data                 information for the creation of the new device
   //--------------------------------------------------------------
   void createDeviceManually(boost::shared_ptr<yApi::IYPluginApi> api,
                             const yApi::IManuallyDeviceCreationData& data);

   //--------------------------------------------------------------
   /// \brief	    getIOManager
   /// \return     the IOManager
   //--------------------------------------------------------------
   boost::shared_ptr<CIOManager> getIOManager() const;

   //--------------------------------------------------------------
   /// \brief	                     Process a command received from Yadoms
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] deviceRemoved    The name of the device removed
   //--------------------------------------------------------------
   void removeDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                     const std::string& deviceRemoved);

   //--------------------------------------------------------------
   /// \brief	                     Change the device configuration
   /// \param [in] name             name of the device
   /// \param [in] newConfiguration new slot position of the device
   //--------------------------------------------------------------
   void onDeviceConfigurationChange(const std::string& name,
                                    const shared::CDataContainerSharedPtr& newConfiguration);

private:

   //--------------------------------------------------------------
   /// \brief	Manager for all IOs
   //--------------------------------------------------------------
   boost::shared_ptr<CIOManager> m_ioManager;

   //--------------------------------------------------------------
   /// \brief	X8R Slots used
   //--------------------------------------------------------------
   bool X8RSlotused[6];

   //--------------------------------------------------------------
   /// \brief	X8D et X24D Slots used
   //--------------------------------------------------------------
   bool X8DSlotused[6];

   //--------------------------------------------------------------
   /// \brief	All extensions
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<equipments::IEquipment>> m_devicesList;
};

