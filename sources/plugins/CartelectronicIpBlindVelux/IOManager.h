#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "equipments/IEquipment.h"
#include "equipments/specificHistorizers/deviceStatus.h"

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
   /// \param[in]  deviceList   list of all devices managed by this plugin
   //--------------------------------------------------------------
   explicit CIOManager(std::vector<boost::shared_ptr<equipments::IEquipment> >& deviceList,
                       std::vector<std::string>& deviceToRetry);

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
   /// \brief	                     try to connect missing equipments
   /// \param [in] api                   Plugin execution context (Yadoms API)
   /// \param [in] pluginConfiguration   plugin configuration
   //--------------------------------------------------------------
   void tryMissingEquipment(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief	                     Process a command received from Yadoms
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] deviceRemoved    the name of the device removed
   //--------------------------------------------------------------
   void removeDevice(boost::shared_ptr<yApi::IYPluginApi> api, 
                     const std::string& deviceRemoved);

   //--------------------------------------------------------------
   /// \brief	    OnConfigurationUpdate
   /// \param[in] api                  yPluginApi API
   /// \param[in] deviceName              the device Name
   /// \param[in] newConfiguration        the new plugin configuration
   //--------------------------------------------------------------
   void OnDeviceConfigurationUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                                    const std::string& deviceName,
                                    const shared::CDataContainer& newConfiguration,
                                    const int refreshEvent);

   //--------------------------------------------------------------
   /// \brief	                     read all devices
   /// \param [in] api                   Plugin execution context (Yadoms API)
   /// \param [in] forceHistorization    force the historization of all keywords
   //--------------------------------------------------------------
   void readAllDevices(boost::shared_ptr<yApi::IYPluginApi> api, bool forceHistorization = false);

   //--------------------------------------------------------------
   /// \brief	    addEquipment
   /// \param[in]  equipment              add a new equipment
   //--------------------------------------------------------------
   void addEquipment(boost::shared_ptr<equipments::IEquipment> equipment);

   //--------------------------------------------------------------
   /// \brief	    getEquipment
   /// \return the number of servers
   //--------------------------------------------------------------
   int getEquipment() const;

   //--------------------------------------------------------------
   /// \brief	    getMasterEquipment
   /// \return the number of servers
   //--------------------------------------------------------------
   int getWaitingEquipment() const;

   //--------------------------------------------------------------
   /// \brief	    check if a device with the same name already exist into the CioManager
   /// \return true if already exist
   //--------------------------------------------------------------
   bool deviceAlreadyExist(const std::string& deviceName);

   //--------------------------------------------------------------
   /// \brief	    getdeviceStates
   /// \return the number of servers
   //--------------------------------------------------------------
   std::vector<specificHistorizers::EdeviceStatus> getDeviceStates();

   //--------------------------------------------------------------
   /// \brief	    bindMasterDevice
   /// \return a container with all servers equipments that could be used to add a sub-equipment
   //--------------------------------------------------------------
   shared::CDataContainer bindMasterDevice();

private:

   //--------------------------------------------------------------
   /// \brief The device Manager
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<equipments::IEquipment>> m_deviceManager;

   //--------------------------------------------------------------
   /// \brief The device Manager
   //--------------------------------------------------------------
   std::vector<std::string> m_deviceToRetry;
};