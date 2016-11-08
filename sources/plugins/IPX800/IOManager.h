#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IOManager.h"
#include <shared/event/EventHandler.hpp>
#include <Poco/Net/NetworkInterface.h>
#include "specificHistorizers/Analog.h"
#include "specificHistorizers/inputOutput.h"
#include "specificHistorizers/counter.h"
#include "extensions/IExtension.h"

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
   // TODO : VOir pour éventuelle créer un device IPX800 et le faire passer dedans directement, cela sera peut-être le plus simple
   void Initialize(boost::shared_ptr<yApi::IYPluginApi> api, 
                   std::vector<boost::shared_ptr<specificHistorizers::CInputOuput> >& RelayList,
                   std::vector<boost::shared_ptr<specificHistorizers::CInputOuput> >& DIList,
                   std::vector<boost::shared_ptr<specificHistorizers::CAnalog> >& analogList,
                   std::vector<boost::shared_ptr<specificHistorizers::CCounter> >& counterList,
                   std::vector<boost::shared_ptr<extensions::IExtension> >& extensionList
                   );

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

   template<typename T1, typename T2>
   void readIOFromDevice(boost::shared_ptr<yApi::IYPluginApi> api, 
                         const std::string& type,
                         const std::vector<boost::shared_ptr<T1> >& list);

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
   /// \brief	Map of all Relays
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<specificHistorizers::CInputOuput> > m_RelayList;

   //--------------------------------------------------------------
   /// \brief	Map of all DI
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<specificHistorizers::CInputOuput> > m_DIList;

   //--------------------------------------------------------------
   /// \brief	Map of all virtual analog input
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<specificHistorizers::CAnalog> > m_analogList;

   //--------------------------------------------------------------
   /// \brief	Map of all counters
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<specificHistorizers::CCounter> > m_countersList;

   //--------------------------------------------------------------
   /// \brief	All extensions
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<extensions::IExtension> > m_devicesList;
};
