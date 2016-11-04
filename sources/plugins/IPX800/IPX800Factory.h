#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/event/EventHandler.hpp>
#include "IIPX800Configuration.h"
#include "IOManager.h"
#include "specificHistorizers/Analog.h"
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationRequest.h>
#include "specificHistorizers/inputOutput.h"
//#include "extensions/IDevice.h"

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
   /// \brief	    bindSlotsX8D
   //--------------------------------------------------------------
   shared::CDataContainer bindSlotsX8D();

   //--------------------------------------------------------------
   /// \brief	    bindSlotsX24D
   //--------------------------------------------------------------
   shared::CDataContainer bindSlotsX24D();

   //--------------------------------------------------------------
   /// \brief	    createDeviceManually
   /// \param[in] api                  yPluginApi API
   //--------------------------------------------------------------
   std::string createDeviceManually(boost::shared_ptr<yApi::IYPluginApi> api, 
                                    const yApi::IManuallyDeviceCreationData& data);

   //--------------------------------------------------------------
   /// \brief	    getIOManager
   /// \return     the IOManager
   //--------------------------------------------------------------
   boost::shared_ptr<CIOManager> getIOManager(void);

private:

   //--------------------------------------------------------------
   /// \brief	Map of all relays
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<specificHistorizers::CInputOuput> > m_relaysList;

   //--------------------------------------------------------------
   /// \brief	Map of all Digital input
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<specificHistorizers::CInputOuput> > m_DIList;

   //--------------------------------------------------------------
   /// \brief	Map of all Analog input
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<specificHistorizers::CAnalog> > m_analogList;

   //--------------------------------------------------------------
   /// \brief	Map of all counters
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<yApi::historization::CCounter> > m_countersList;

   //--------------------------------------------------------------
   /// \brief	Manager for all IOs
   //--------------------------------------------------------------
   boost::shared_ptr<CIOManager> m_ioManager;

   //--------------------------------------------------------------
   /// \brief	X8R Slots used
   //--------------------------------------------------------------
   bool X8RSlotused[6];

   //--------------------------------------------------------------
   /// \brief	X8D Slots used
   //--------------------------------------------------------------
   bool X8DSlotused[6];

   //--------------------------------------------------------------
   /// \brief	X24D Slots used
   //--------------------------------------------------------------
   bool X24DSlotused[2];
   //--------------------------------------------------------------
   /// \brief	All extensions
   //--------------------------------------------------------------
   //std::vector<boost::shared_ptr<devices::IDevice> > m_devicesList;
};