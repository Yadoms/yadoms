#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IO.h"
#include "IOManager.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

enum
{
   kEvtIOStateReceived = yApi::IYPluginApi::kPluginFirstEventId // Always start from shared::event::CEventHandler::kUserFirstId
};

//--------------------------------------------------------------
/// \brief	Class where all keywords are updated
/// \note   
//--------------------------------------------------------------
class CIOManager
{
public:
   //--------------------------------------------------------------
   /// \brief	   Constructor
   /// \param[in] device              The device name
   //--------------------------------------------------------------
   CIOManager( const std::string& device );

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
   /// \brief	    SendMessage
   /// \param[in] Event             Information to send
   //--------------------------------------------------------------
   //static void SendMessage(CIOState Event);

   //--------------------------------------------------------------
   /// \brief	   setNewIOList
   /// \param[in] IOlist            Set a new list of IOs
   //--------------------------------------------------------------
   void setNewIOList(std::map<std::string, boost::shared_ptr<CIO> > IOlist);

private:

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
};
