#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IO.h"
#include "IOManager.h"
#include <shared/event/EventHandler.hpp>

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
   /// \param[in] device                The device name
   //--------------------------------------------------------------
   explicit CIOManager(const std::string& device);

   //--------------------------------------------------------------
   /// \brief	    Initialize all elements
   /// \param[in]  api              Plugin execution context (Yadoms API)
   /// \param[in]  IOlist           Set a new list of IOs
   //--------------------------------------------------------------
   void Initialize(boost::shared_ptr<yApi::IYPluginApi> api, 
                   std::map<std::string, boost::shared_ptr<CIO> > IOlist);

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
   void onCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                  int receivedValue);

private:

   //--------------------------------------------------------------
   /// \brief	The event handler to notify for events ( interrupt due to change value )
   //--------------------------------------------------------------
   shared::event::CEventHandler *m_InterruptEventHandler;

   //--------------------------------------------------------------
   /// \brief	The plugin name
   //--------------------------------------------------------------
   std::string m_deviceName;

   //--------------------------------------------------------------
   /// \brief	The keywords lists to historize in one step for better performances
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywordsToDeclare;

   //--------------------------------------------------------------
   /// \brief	Map of all IOs identify by the name
   //--------------------------------------------------------------
   std::map<std::string, boost::shared_ptr<CIO> > m_mapKeywordsName;

   //--------------------------------------------------------------
   ///\brief   The thread used to receive messages from Yadoms
   //--------------------------------------------------------------
   boost::thread m_interruptReceiverThread;

   //--------------------------------------------------------------
   ///\brief   The thread function to receive interrupts from the component
   //--------------------------------------------------------------
   void interruptReceiverThreaded(const std::string& keywordName) const;

   //--------------------------------------------------------------
   ///\brief   Values of the DI
   //--------------------------------------------------------------
   char m_inputValue;
   
   bool m_initializationOk;
};
