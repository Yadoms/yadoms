#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "Configuration.h"
#include "IDeviceManager.h"

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
	boost::shared_ptr<IDeviceManager> m_deviceManager;

	static void declareDevice(boost::shared_ptr<yApi::IYPluginApi>& api,
	                          boost::shared_ptr<UsbDeviceInformation>& deviceInformation);

	boost::shared_ptr<UsbDeviceInformation> initDevice(boost::shared_ptr<yApi::IYPluginApi>& api) const;

	std::shared_ptr<unsigned char> allocString(const char* pString, size_t& outSize);
};
