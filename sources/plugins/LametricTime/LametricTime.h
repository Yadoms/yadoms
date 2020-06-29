#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include <boost/smart_ptr/shared_ptr.hpp>
#include "Configuration.h"
#include "IDeviceState.h"
#include "INotificationSender.h"
#include "SsdpDiscoverService.h"
#include "CustomizeIconType.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

struct DeviceInformation
{
   std::string m_deviceName;
   std::string m_deviceType;
   std::string m_deviceModel;
   std::string m_deviceIp;
};

class CLametricTime : public plugin_cpp_api::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CLametricTime();
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CLametricTime() = default;

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

private:
   /**
    * \brief Declare device
    */
   void declareDevice() const;
   /**
    * \brief  Declare Keyword
    */
   void declareKeyword() const;
   /**
    * \brief Fill device information manually
    * \return     deviceInformation     Structure with device information
    */
   void fillDeviceInformationManually() const;
   /**
    * \brief Fill devices information automatically
    * \param[in]  foundDevices                       Class containing device information
    * \return     std::vector<DeviceInformation>     vector of structure containing device information
    */
   static std::vector<DeviceInformation> fillAllDevicesInformationAutomatically(
      const std::vector<boost::shared_ptr<CSsdpDiscoveredDevice>>& foundDevices);
   /**
    * \brief Init Lametric device
    */
   void init();
   /**
   * \brief Init Lametric device using UPNP protocol
   * \return    std::vector<DeviceInformation>        vector of structure containing device information
   */
   std::vector<DeviceInformation> initAutomatically() const;
   /**
    * \brief Init Lametric device 
    * \return                         Structure with device information
   */
   void initManually();
   /**
    * \brief Update the configuration of the plugin after a change
   * \param newConfigurationData The new configuration of the module
   */
   void onUpdateConfiguration(const boost::shared_ptr<shared::CDataContainer>& newConfigurationData);

   /**
   * \brief Declare all devices and keywords
   * \param devicesInformation   vector of structure containing device information
   */
   void declareAllDevicesAndKeywords(std::vector<DeviceInformation>& devicesInformation) const;
   /**
   * \brief Retry to init Manually the device in case of a none Unauthorized http code status
   */
   void retryInitManually();
   /**
   * \brief sending echo to device & fill and declare device(s) and keyword(s)
   */
   void createDevice() const;
   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   CConfiguration m_configuration;

   boost::shared_ptr<IDeviceState> m_deviceManager;
   boost::shared_ptr<INotificationSender> m_senderManager;
   boost::shared_ptr<yApi::historization::CText> m_text;
   boost::shared_ptr<specificHistorizers::CCustomizeIconType> m_iconType;
   boost::shared_ptr<DeviceInformation> m_deviceInformation;
   std::vector<DeviceInformation> m_devicesInformation;
   boost::shared_ptr<yApi::IYPluginApi> m_api;


   std::vector<DeviceInformation>::iterator m_targetDevice;
   static const std::string DeviceName;
   static const std::string TextKeywordName;
   static const std::string IconTypeName;
};
