#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "LametricConfiguration.h"
#include "LametricDeviceState.h"
#include <boost/smart_ptr/shared_ptr.hpp>
#include "LametricNotificationSender.h"


// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

struct DeviceInformation
{
   std::string deviceName;
   std::string deviceType;
   std::string deviceModel;
};

class CLametric : public plugin_cpp_api::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CLametric();
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CLametric() = default;

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

private:
   /**
    * \brief Declare device
    * \param[in] api                   Pointer to the API
    * \param[in] deviceInformation     Structure with device informations
    */
   static void declareDevice(boost::shared_ptr<yApi::IYPluginApi>& api, DeviceInformation& deviceInformation);
   /**
    * \brief  Declare Keyword
    * \param api pointer to the API
    * \param[in] deviceInformation     Structure with device informations
    */
   void declareKeyword(boost::shared_ptr<yApi::IYPluginApi>& api, DeviceInformation& deviceInformation) const;
   /**
    * \brief Fill device informations
    * \param[in] deviceInformation     Structure with device informations
    */
   void fillDeviceInformation(DeviceInformation& deviceInformation) const;
   /**
    * \brief Init Lametric device
    * \param[in] api                   Pointer to the API
    * \param[int] deviceInformation    Structure with device informations
    */
   void initLametric(boost::shared_ptr<yApi::IYPluginApi>& api, DeviceInformation& deviceInformation) const;
   /**
    * \brief Update the configuration of the plugin after a change
    * \param api                  pointer to the API
    * \param newConfigurationData The new configuration of the module
    */
   void onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                              const shared::CDataContainer& newConfigurationData);

   //--------------------------------------------------------------
   /// \brief	Refresh timer
   //--------------------------------------------------------------
   boost::shared_ptr<shared::event::CEventTimer> m_refreshTimer;
   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   CLametricConfiguration m_configuration;

   boost::shared_ptr<CLametricDeviceState> m_lametricDeviceManager;
   boost::shared_ptr<CLametricNotificationSender> m_lametricManagerSender;
   boost::shared_ptr<yApi::historization::CText> m_text;

   static const std::string DeviceName;
   static const std::string TextKeywordName;
};
