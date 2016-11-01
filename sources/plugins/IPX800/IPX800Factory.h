#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/event/EventHandler.hpp>
//#include "IIO.h"
//#include "IO.h"
#include "IIPX800Configuration.h"
#include "IOManager.h"
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/NetworkInterface.h>

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
   /// \param[in] details             Details information for keyword
   //--------------------------------------------------------------
   CIPX800Factory(boost::shared_ptr<yApi::IYPluginApi> api,
                  const std::string& device,
                  const IIPX800Configuration& configuration,
                  shared::CDataContainer details);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CIPX800Factory();

   //--------------------------------------------------------------
   /// \brief	    OnConfigurationUpdate
   /// \param[in] api                 yPluginApi API
   /// \param[in] ISIConfiguration    The new configuration
   /// \param[in] details             Details information for keyword
   //--------------------------------------------------------------
   void OnConfigurationUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                              const IIPX800Configuration& configuration,
                              shared::CDataContainer details);

   //--------------------------------------------------------------
   /// \brief	    getIOManager
   /// \return     the IOManager
   //--------------------------------------------------------------
   boost::shared_ptr<CIOManager> getIOManager(void);

private:

   //--------------------------------------------------------------
   /// \brief	The keywords lists to historize in one step for better performances
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywordsToDeclare;

   //--------------------------------------------------------------
   /// \brief	Map of all IOs identify by the name
   //--------------------------------------------------------------
   std::map<std::string, boost::shared_ptr<yApi::historization::CSwitch> > m_mapKeywordsName;

   //--------------------------------------------------------------
   /// \brief	Manager for all IOs
   //--------------------------------------------------------------
   boost::shared_ptr<CIOManager> m_ioManager;
};