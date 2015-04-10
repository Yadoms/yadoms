#pragma once
#include <shared/plugin/IPlugin.h>
#include "SIConfiguration.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This class is the WindowsSystem plugin
/// \note   This plugin send periodically :
/// \note   - The memory load of the Windows System
/// \note   - The CPU load od the Windows System
/// \note   - The CPU load of Yadoms application
/// \note   - Disks Usages of all fixed disks (HDD)
//--------------------------------------------------------------
class CSystemInformation : public shared::plugin::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CSystemInformation();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CSystemInformation();

   // IPlugin implementation
   virtual void doWork(boost::shared_ptr<yApi::IYPluginApi> context);
   // [END] IPlugin implementation

   //--------------------------------------------------------------
   /// \brief Update the configuration of the plugin after a change
   /// \param[in] context               pointer to the API
   /// \param[in] newConfigurationData  The new configuration of the module
   //--------------------------------------------------------------
   void onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> context, const shared::CDataContainer& newConfigurationData);

private:
   std::string m_deviceName;

   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   CSIConfiguration m_configuration;
};