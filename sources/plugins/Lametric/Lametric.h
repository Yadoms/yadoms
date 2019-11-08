#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include <boost/smart_ptr/shared_ptr.hpp>
#include "Configuration.h"
#include "IDeviceState.h"
#include "INotificationSender.h"

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
    * \param[in] deviceInformation     Structure with device information
    */
   static void declareDevice(boost::shared_ptr<yApi::IYPluginApi>& api,
                             boost::shared_ptr<DeviceInformation>& deviceInformation);
   /**
    * \brief  Declare Keyword
    * \param api pointer to the API
    * \param[in] deviceInformation     Structure with device information
    */
   void declareKeyword(boost::shared_ptr<yApi::IYPluginApi>& api,
                       boost::shared_ptr<DeviceInformation>& deviceInformation) const;
   /**
    * \brief Fill device information
    * \param[in] deviceInformation     Structure with device information
    */
   void fillDeviceInformation(boost::shared_ptr<DeviceInformation>& deviceInformation) const;
   /**
    * \brief Init Lametric device
    * \param[in] api                   Pointer to the API
    * \return                          Structure with device information
    */
   boost::shared_ptr<DeviceInformation> initLametric(boost::shared_ptr<yApi::IYPluginApi>& api) const;
   /**
    * \brief Update the configuration of the plugin after a change
    * \param api                  pointer to the API
    * \param newConfigurationData The new configuration of the module
    */
   void onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                              const shared::CDataContainer& newConfigurationData);

   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   CConfiguration m_configuration;

   boost::shared_ptr<IDeviceState> m_deviceManager;
   boost::shared_ptr<INotificationSender> m_managerSender;
   boost::shared_ptr<yApi::historization::CText> m_text;

   static const std::string DeviceName;
   static const std::string TextKeywordName;
};
