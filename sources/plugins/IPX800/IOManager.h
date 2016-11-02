#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
//#include "IIO.h"
#include "IOManager.h"
#include <shared/event/EventHandler.hpp>
#include <Poco/Net/NetworkInterface.h>
#include "specificHistorizers/Analog.h"

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
   /// \param[in] device            The device name
   /// \param[in] IPAddress         The IP Address of the IPX800
   /// \param[in] passwordActivated If the password is used
   /// \param[in] password          password used to access the equipment
   //--------------------------------------------------------------
   explicit CIOManager(const std::string& device, 
                       Poco::Net::IPAddress IPAddress, 
                       bool passwordActivated, 
                       std::string password);

   //--------------------------------------------------------------
   /// \brief	    Initialize all elements
   /// \param[in]  api              Plugin execution context (Yadoms API)
   /// \param[in]  IOlist           Set a new list of IOs
   //--------------------------------------------------------------
   void Initialize(boost::shared_ptr<yApi::IYPluginApi> api, 
                   std::map<std::string, boost::shared_ptr<yApi::historization::CSwitch> > IOlist,
                   std::map<std::string, boost::shared_ptr<specificHistorizers::CVirtualAnalog> > virtualAnalogList,
                   std::map<std::string, boost::shared_ptr<yApi::historization::CCounter> > counterList);

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
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] receivedValue    The received value from the interrupt
   //--------------------------------------------------------------
   //void onCommand(boost::shared_ptr<yApi::IYPluginApi> api,
   //               int receivedValue);
   void readIOFromDevice();

private:

   //--------------------------------------------------------------
   /// \brief	The plugin name
   //--------------------------------------------------------------
   std::string m_deviceName;

   //--------------------------------------------------------------
   /// \brief	IP Address of the equipment
   //--------------------------------------------------------------
   Poco::Net::IPAddress m_IPAddress;

   //--------------------------------------------------------------
   /// \brief	Password activation
   //--------------------------------------------------------------
   bool m_isPasswordActivated;

   //--------------------------------------------------------------
   /// \brief	Password
   //--------------------------------------------------------------
   std::string m_password;

   //--------------------------------------------------------------
   /// \brief	The keywords lists to historize in one step for better performances
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywordsToDeclare;

   //--------------------------------------------------------------
   /// \brief	Map of all IOs identify by the name
   //--------------------------------------------------------------
   std::map<std::string, boost::shared_ptr<yApi::historization::CSwitch> > m_mapDigitalInputOuput;

   //--------------------------------------------------------------
   /// \brief	Map of all virtual analog input
   //--------------------------------------------------------------
   std::map<std::string, boost::shared_ptr<specificHistorizers::CVirtualAnalog> > m_mapVirtualAnalogInput;

   //--------------------------------------------------------------
   /// \brief	Map of all counters
   //--------------------------------------------------------------
   std::map<std::string, boost::shared_ptr<yApi::historization::CCounter> > m_mapCounters;
};
