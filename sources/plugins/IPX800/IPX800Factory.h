#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/event/EventHandler.hpp>
#include "IIPX800Configuration.h"
#include "IOManager.h"
//#include <Poco/Net/SocketAddress.h>
//#include <Poco/Net/NetworkInterface.h>
#include "specificHistorizers/Analog.h"
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationRequest.h>
#include "devices/IDevice.h"

#define IPX800_RELAY_QTY 8
#define IPX800_DI_QTY    8
#define IPX800_ANA_QTY   4
#define IPX800_CNT_QTY  16

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
   //--------------------------------------------------------------
   CIPX800Factory(boost::shared_ptr<yApi::IYPluginApi> api,
                  const std::string& device,
                  const IIPX800Configuration& configuration);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CIPX800Factory();

   //--------------------------------------------------------------
   /// \brief	    OnConfigurationUpdate
   /// \param[in] api                  yPluginApi API
   /// \param[in] IIPX800Configuration configuration
   //--------------------------------------------------------------
   void OnConfigurationUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                              const IIPX800Configuration& configuration);

   //--------------------------------------------------------------
   /// \brief	    createIPX800Device
   /// \param[in] api                  yPluginApi API
   //--------------------------------------------------------------
   void createIPX800Device(boost::shared_ptr<yApi::IYPluginApi> api,
                           const std::string& device);

   //--------------------------------------------------------------
   /// \brief	    bindSlotsX8R
   //--------------------------------------------------------------
   shared::CDataContainer bindSlotsX8R();

   //--------------------------------------------------------------
   /// \brief	    createDeviceManually
   /// \param[in] api                  yPluginApi API
   //--------------------------------------------------------------
   std::string createDeviceManually(boost::shared_ptr<yApi::IYPluginApi> api, 
                                    const yApi::IManuallyDeviceCreationData& data) const;

   //--------------------------------------------------------------
   /// \brief	    getIOManager
   /// \return     the IOManager
   //--------------------------------------------------------------
   boost::shared_ptr<CIOManager> getIOManager(void);

private:
   //--------------------------------------------------------------
   /// \brief	    createX8RDevice
   /// \param[in] api                  yPluginApi API
   //--------------------------------------------------------------
   void createX8RDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                        const std::string& device);

   //--------------------------------------------------------------
   /// \brief	    createX8DDevice
   /// \param[in] api                  yPluginApi API
   //--------------------------------------------------------------
   void createX8DDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                        const std::string& device);

   //--------------------------------------------------------------
   /// \brief	    createX24DDevice
   /// \param[in] api                  yPluginApi API
   //--------------------------------------------------------------
   void createX24DDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                         const std::string& device);

   //--------------------------------------------------------------
   /// \brief	Map of all IOs identify by the name (Relays, virtual inputs, virtuals outputs)
   //--------------------------------------------------------------
   std::map<std::string, boost::shared_ptr<yApi::historization::CSwitch> > m_mapDigitalInputOuput;

   //--------------------------------------------------------------
   /// \brief	Map of all virtual analog input
   //--------------------------------------------------------------
   std::map<std::string, boost::shared_ptr<specificHistorizers::CAnalog> > m_mapVirtualAnalogInput;

   //--------------------------------------------------------------
   /// \brief	Map of all counters
   //--------------------------------------------------------------
   std::map<std::string, boost::shared_ptr<yApi::historization::CCounter> > m_mapCounters;

   //--------------------------------------------------------------
   /// \brief	Manager for all IOs
   //--------------------------------------------------------------
   boost::shared_ptr<CIOManager> m_ioManager;

   std::vector<boost::shared_ptr<devices::IDevice> > m_devicesList;
};