#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/event/EventHandler.hpp>
#include "IO.h"
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
                   shared::CDataContainer details);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CPiface2Factory();

   //--------------------------------------------------------------
   /// \brief	    OnConfigurationUpdate
   /// \param[in] api                 yPluginApi API
   /// \param[in] ISIConfiguration    The new configuration
   /// \param[in] details             Details information for keyword
   //--------------------------------------------------------------
   void OnConfigurationUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                              const IPf2Configuration& configuration,
                              shared::CDataContainer details);

   std::map<std::string, boost::shared_ptr<CIO> >getAllDigitalIO(void);

private:

   //--------------------------------------------------------------
   /// \brief	The keywords lists to historize in one step for better performances
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywordsToDeclare;

   //--------------------------------------------------------------
   /// \brief	Map of all IOs identify by the name
   //--------------------------------------------------------------
   std::map<std::string, boost::shared_ptr<CIO> > m_mapKeywordsName;

   //--------------------------------------------------------------
   /// \brief	IOs of the Piface2
   //--------------------------------------------------------------
   boost::shared_ptr<CIO> m_DigitalInput[8];
   boost::shared_ptr<CIO> m_DigitalOutput[8];
};