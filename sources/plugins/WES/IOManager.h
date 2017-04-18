#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/event/EventHandler.hpp>
#include "equipments/IEquipment.h"
#include "IWESConfiguration.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Class where all keywords are updated
/// \note   
//--------------------------------------------------------------
class CIOManager
{
public:
   //--------------------------------------------------------------
   /// \brief	   Constructor
   /// \param[in]  extensionList   Set a new equipment
   //--------------------------------------------------------------
   explicit CIOManager(std::vector<boost::shared_ptr<equipments::IEquipment> >& deviceList,
                       std::vector<boost::shared_ptr<equipments::IEquipment> >& masterDeviceList);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CIOManager();

   //--------------------------------------------------------------
   /// \brief	                     Process a command received from Yadoms
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] command          The received command
   //--------------------------------------------------------------
   void onCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                  boost::shared_ptr<const yApi::IDeviceCommand> command);

   //--------------------------------------------------------------
   /// \brief	                     Process a command received from Yadoms
   /// \param [in] api                   Plugin execution context (Yadoms API)
   /// \param [in] receivedValue         The received value from the interrupt
   /// \param [in] forceHistorization    force the historization of all keywords
   //--------------------------------------------------------------
   void readIOFromDevice(boost::shared_ptr<yApi::IYPluginApi> api, 
                         const std::string& type,
                         bool forceHistorization = false);

   //--------------------------------------------------------------
   /// \brief	                     Process a command received from Yadoms
   /// \param [in] api                   Plugin execution context (Yadoms API)
   /// \param [in] forceHistorization    force the historization of all keywords
   //--------------------------------------------------------------
   void readAllDevices(boost::shared_ptr<yApi::IYPluginApi> api, 
                       const boost::shared_ptr<IWESConfiguration> pluginConfiguration,
                       bool forceHistorization = false);

   //--------------------------------------------------------------
   /// \brief	                     Process a command received from Yadoms
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] deviceRemoved    the name of the device removed
   //--------------------------------------------------------------
   void removeDevice(boost::shared_ptr<yApi::IYPluginApi> api, std::string deviceRemoved);

   //--------------------------------------------------------------
   /// \brief	    OnConfigurationUpdate
   /// \param[in] api                  yPluginApi API
   /// \param[in] configuration        the new plugin configuration
   //--------------------------------------------------------------
   void OnDeviceConfigurationUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                                    const std::string &deviceName,
                                    const shared::CDataContainer newConfiguration);

   //--------------------------------------------------------------
   /// \brief	    getMasterEquipment
   /// \return the number of master equipment
   //--------------------------------------------------------------
   void addEquipment(boost::shared_ptr<equipments::IEquipment> equipment);

   //--------------------------------------------------------------
   /// \brief	    getMasterEquipment
   /// \return the number of master equipment
   //--------------------------------------------------------------
   int getMasterEquipment();

   //--------------------------------------------------------------
   /// \brief	    bindMasterDevice
   /// \return the number of master equipment
   //--------------------------------------------------------------
   shared::CDataContainer bindMasterDevice();

private:

   //--------------------------------------------------------------
   /// \brief The device Manager
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<equipments::IEquipment>> m_deviceManager;

   //--------------------------------------------------------------
   /// \brief The master device Manager
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<equipments::IEquipment>> m_masterDeviceManager;
};