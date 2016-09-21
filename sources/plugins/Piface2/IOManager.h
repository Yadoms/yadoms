#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IO.h"
#include "IOManager.h"
#include <shared/event/EventHandler.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	the structure sent with the event
//--------------------------------------------------------------
typedef struct CIOState
{
   std::string keywordName;
   unsigned char value;
}IOState;

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
   /// \param[in] interruptEventHandler interrupt Event handler to read values.
   //--------------------------------------------------------------
   CIOManager( const std::string& device, 
               shared::event::CEventHandler& interruptEventHandler);

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
                  boost::shared_ptr<const yApi::IDeviceCommand> command,
                  bool fromInput);

   //--------------------------------------------------------------
   /// \brief	   setNewIOList
   /// \param[in] IOlist            Set a new list of IOs
   //--------------------------------------------------------------
   void setNewIOList(std::map<std::string, boost::shared_ptr<CIO> > IOlist);

private:

   //--------------------------------------------------------------
   /// \brief	The event handler to notify for events ( interrupt due to change value )
   //--------------------------------------------------------------
   shared::event::CEventHandler& m_InterruptEventHandler;

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
};
