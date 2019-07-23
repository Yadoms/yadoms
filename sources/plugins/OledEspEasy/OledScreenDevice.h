#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "OledEspEasyController.h"
#include "OledScreenLine.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;


//--------------------------------------------------------------
/// \brief	Configuration of the empty plugin
//--------------------------------------------------------------
class COledScreenDevice
{
public:
	COledScreenDevice(boost::shared_ptr<COledEspEasyController> controller);
	COledScreenDevice(boost::shared_ptr<COledEspEasyController> controller, std::string deviceName);
	virtual ~COledScreenDevice();

	const std::string& getDeviceName() const;
	void declareDevice(boost::shared_ptr<yApi::IYPluginApi> & yApi);

	void updateScreen(const std::string& keyword, const std::string& text);
private:
	boost::shared_ptr<COledEspEasyController> m_controller;
	std::string m_deviceName;
	std::vector< boost::shared_ptr<COledScreenLine> > m_keywordLines;
};

