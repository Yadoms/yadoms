#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "HttpController.h"
#include "IRealScreen.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;


//--------------------------------------------------------------
/// \brief	Configuration of the empty plugin
//--------------------------------------------------------------
class CScreenDevice
{
public:
	CScreenDevice(const CConfiguration & config, boost::shared_ptr<CHttpController> controller, std::string deviceName);
	virtual ~CScreenDevice();

	const std::string& getDeviceName() const;
	void declareDevice(boost::shared_ptr<yApi::IYPluginApi> & yApi);

	void updateScreen(const std::string& keyword, const std::string& screenCommands);
private:
	boost::shared_ptr<CHttpController> m_controller;
	std::string m_deviceName;
	boost::shared_ptr< yApi::historization::CText> m_update_screen_event;

	boost::shared_ptr<IRealScreen> m_realScreen;
};

