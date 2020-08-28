#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "Configuration.h"
#include "DeviceManagerHelper.h"
#include "DefaultIconSelector.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This class is an empty plugin example
/// \note   This plugin do nothing, you just have to :
///         - rename all classes of this plugin
///         - populate all code/functions
//--------------------------------------------------------------
class CStreamDeck : public plugin_cpp_api::IPlugin
{
public:
	//--------------------------------------------------------------
	/// \brief	Constructor
	//--------------------------------------------------------------
	CStreamDeck();

	//--------------------------------------------------------------
	/// \brief	Destructor
	//--------------------------------------------------------------
	virtual ~CStreamDeck();

	// IPlugin implementation
	void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
	// [END] IPlugin implementation

private:
	//--------------------------------------------------------------
	/// \brief	The plugin configuration
	//--------------------------------------------------------------
	CConfiguration m_configuration;
	boost::shared_ptr<CDeviceManager> m_deviceManager;

	void declareDeviceAndKeywords(boost::shared_ptr<yApi::IYPluginApi>& api);


	void initDevice(boost::shared_ptr<yApi::IYPluginApi>& api);

	boost::shared_ptr<yApi::historization::CEvent> m_keyStateEvent;

	boost::shared_ptr<UsbDeviceInformation> m_usbDeviceInformation;

	std::map<int, boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>> m_keywords;

};
