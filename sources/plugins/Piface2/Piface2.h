#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "Pf2Configuration.h"
#include "IOManager.h"
#include "Piface2Factory.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//-----------------------------------------------------
///\brief The plugin state
//-----------------------------------------------------

enum EPiface2PluginState
{
   kUndefined = 0,
   kInitializationError,
   kupdateConfiguration,
   kSPIError,
   kRunning,
   kStop
};

//--------------------------------------------------------------
/// \brief	This class is the Piface2 plugin
/// \note   This plugin configure GPIOs of the Pi board for using the Piface2 extension board.
//--------------------------------------------------------------
class CPiface2 : public plugin_cpp_api::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CPiface2();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CPiface2();

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

   //--------------------------------------------------------------
   /// \brief	   onUpdateConfiguration
   /// \param[in] api                  Yadoms api
   /// \param[in] newConfigurationData The new configuration
   //--------------------------------------------------------------
   void onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api, 
                              const shared::CDataContainer& newConfigurationData,
                              shared::CDataContainer& details);

protected:

private:

   //-----------------------------------------------------
   ///\brief                                      set the new plugin state
   ///\param[in] api                             Yadoms API
   ///\param[in] newState                        the new plugin state
   //-----------------------------------------------------
   void setPluginState(boost::shared_ptr<yApi::IYPluginApi> api,
                       EPiface2PluginState newState);

   //--------------------------------------------------------------
   /// \brief	The device name
   //--------------------------------------------------------------
   std::string m_deviceName;

   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   CPf2Configuration m_configuration;

   //--------------------------------------------------------------
   /// \brief	The IO Manager
   //--------------------------------------------------------------
   boost::shared_ptr<CIOManager> m_ioManager;

   //--------------------------------------------------------------
   /// \brief	The factory
   //--------------------------------------------------------------
   boost::shared_ptr<CPiface2Factory> m_factory;

   //--------------------------------------------------------------
   /// \brief	PluginState
   //--------------------------------------------------------------
   EPiface2PluginState m_pluginState;
};

