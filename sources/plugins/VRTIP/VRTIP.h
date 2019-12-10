#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "VRTIPFactory.h"
#include "IOManager.h"
#include <shared/event/EventTimer.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//-----------------------------------------------------
///\brief The plugin state
//-----------------------------------------------------

enum EVRTIPPluginState
{
   kUndefined = 0,
   kStop,
   kInitialization,
   kUpdateConfiguration,
   kReady,
   kAtLeastOneConnectionFaulty,
   kmanuallyCreationDeviceFailed,
   kRunning
};

//--------------------------------------------------------------
/// \brief	This class is the WES plugin
//--------------------------------------------------------------
class CVRTIP : public plugin_cpp_api::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
	CVRTIP();

   //-----------------------------------------------------
   ///\brief                                      update the configuration
   ///\param[in] api                             Yadoms API
   ///\param[in] keywordsToHistorize             the new plugin configuration
   //-----------------------------------------------------
   void onUpdateConfiguration(
      const shared::CDataContainer& newConfigurationData) const;

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CVRTIP();

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

private:

   //-----------------------------------------------------
   ///\brief                                      analyze the new plugin state
   ///\param[in] api                             Yadoms API
   //-----------------------------------------------------
   void analyzePluginState(boost::shared_ptr<yApi::IYPluginApi> api);

   //-----------------------------------------------------
   ///\brief                                      set the new plugin state
   ///\param[in] api                             Yadoms API
   ///\param[in] newState                        the new plugin state
   //-----------------------------------------------------
   void setPluginState(boost::shared_ptr<yApi::IYPluginApi> api,
                       EVRTIPPluginState newState);

   //--------------------------------------------------------------
   /// \brief The factory
   //--------------------------------------------------------------
   boost::shared_ptr<CVRTIPFactory> m_factory;

   //--------------------------------------------------------------
   /// \brief The IO Manager
   //--------------------------------------------------------------
   boost::shared_ptr<CIOManager> m_ioManager;

   //--------------------------------------------------------------
   /// \brief	Refresh timer
   //--------------------------------------------------------------
   boost::shared_ptr<shared::event::CEventTimer> m_refreshTimer;

   //--------------------------------------------------------------
   /// \brief	PluginState
   //--------------------------------------------------------------
   EVRTIPPluginState m_pluginState;
};