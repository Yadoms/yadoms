#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/event/EventHandler.hpp>
#include "IO.h"
#include "eventIdentification.h"
#include "IPf2Configuration.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Class where all Keywords are created and updated
/// \note   
//--------------------------------------------------------------
class CPiface2Factory
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] api                 yPluginApi API
   /// \param[in] device              The device name
   /// \param[in] details             Details information for keyword
   //--------------------------------------------------------------
   CPiface2Factory(boost::shared_ptr<yApi::IYPluginApi> api,
                   const std::string& device,
                   const IPf2Configuration& configuration,
                   shared::CDataContainer details,
                   int forId);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CPiface2Factory();

   //--------------------------------------------------------------
   /// \brief	                     Process a command received from Yadoms
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] command          The received command
   //--------------------------------------------------------------
   void onCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                  boost::shared_ptr<const yApi::IDeviceCommand> command,
                  bool fromInput);

   //--------------------------------------------------------------
   /// \brief	    OnConfigurationUpdate
   /// \param[in] api                 yPluginApi API
   /// \param[in] ISIConfiguration    The new configuration
   /// \param[in] details             Details information for keyword
   //--------------------------------------------------------------
   void OnConfigurationUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                              const IPf2Configuration& configuration,
                              shared::CDataContainer details);

   //--------------------------------------------------------------
   /// \brief	    m_Event
   /// \note        static EnventHandler used by interrupts
   //--------------------------------------------------------------
   static shared::event::CEventHandler m_Event;

private:

   //--------------------------------------------------------------
   /// \brief	The keywords lists to historize in one step for better performances
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywordsToDeclare;

   std::map<std::string, boost::shared_ptr<CIO> > m_mapKeywordsName;

   boost::shared_ptr<CIO> m_DigitalInput[8];
   boost::shared_ptr<CIO> m_DigitalOutput[8];
};
