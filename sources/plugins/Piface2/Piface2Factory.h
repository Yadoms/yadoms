#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/event/eventHandler.hpp>
#include "IO.h"

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
                   shared::CDataContainer details,
                   int forId);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CPiface2Factory();

protected:

   //--------------------------------------------------------------
   /// \brief	                     Process a command received from Yadoms
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] command          The received command
   //--------------------------------------------------------------
   void onCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                  boost::shared_ptr<const yApi::IDeviceCommand> command);

private:

   //--------------------------------------------------------------
   /// \brief	The keywords lists to historize in one step for better performances
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywordsToDeclare;

   shared::event::CEventHandler m_Event;

   boost::shared_ptr<CIO> m_WriteIO;
   boost::shared_ptr<CIO> m_ReadIO;
};