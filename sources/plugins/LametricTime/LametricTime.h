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
	 * \param[in] api                   Pointer to the API
	 * \param[in] deviceInformation     Structure with device information
	 */
	void declareDevice(
	                          boost::shared_ptr<DeviceInformation>& deviceInformation);
	/**
	 * \brief  Declare Keyword
	 * \param api pointer to the API
	 * \param[in] deviceInformation     Structure with device information
	 */
	void declareKeyword(
	                    boost::shared_ptr<DeviceInformation>& deviceInformation) const;
	/**
	 * \brief Fill device information manually
	 * \return     deviceInformation     Structure with device information
	 */
	boost::shared_ptr<DeviceInformation> fillDeviceInformationManually() const;
	/**
	 * \brief Fill devices information automatically
	 * \param[in]  foundDevices                       Class containing device information
	 * \return     std::vector<DeviceInformation>     vector of structure containing device information
	 */
	static std::vector<DeviceInformation> fillAllDevicesInformationAutomatically(CSsdpDiscoveredDevice& foundDevices);
	/**
	 * \brief Init Lametric device
	 * \param[in] api                   Pointer to the API
	 */
	void init();
	/**
	* \brief Init Lametric device using UPNP protocol
	* \param[in] api												Pointer to the API
	* \return    std::vector<DeviceInformation>        vector of structure containing device information
	*/
	std::vector<DeviceInformation> initAutomatically() const;
	/**
	 * \brief Init Lametric device 
	* \param[in] api                   Pointer to the API
	 * \return                         Structure with device information
	*/
	boost::shared_ptr<DeviceInformation> initManually();
	/**
	 * \brief Update the configuration of the plugin after a change
	* \param api                  pointer to the API
	* \param newConfigurationData The new configuration of the module
	*/
	void onUpdateConfiguration(
	                           const boost::shared_ptr<shared::CDataContainer>& newConfigurationData);

	/**
	* \brief Declare all devices and keywords
	* \param api                  pointer to the API
	* \param foundDevices         Class containing device information
	* \param devicesInformation   vector of structure containing device information
	*/
	void declareAllDevicesAndKeywords(
	                                  CSsdpDiscoveredDevice& foundDevices,
	                                  std::vector<DeviceInformation>& devicesInformation) const;
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
