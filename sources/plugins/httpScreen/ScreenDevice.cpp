#include "stdafx.h"
#include "ScreenDevice.h"
#include <shared/http/HttpMethods.h>

#include "Command.h"
#include "CommandParser.h"

#include "TftScreen.h"
#include "EInkScreen.h"

CScreenDevice::CScreenDevice(const CConfiguration& config, boost::shared_ptr<CHttpController> controller, std::string deviceName)
	: m_controller(controller), m_deviceName(std::move(deviceName)),
	  m_update_screen_event(boost::make_shared<yApi::historization::CEvent>("updateScreen", yApi::EKeywordAccessMode::kGetSet))

{
	if(config.getSceenType() == kTft)
	{
		m_realScreen = boost::make_shared< CTftScreen >(controller);
	}
	else
	{
		m_realScreen = boost::make_shared< CEInkScreen >(controller);
	}

	m_realScreen->setup_ip(config.getIPAddress());
}

CScreenDevice::~CScreenDevice()
{
	
}

const std::string& CScreenDevice::getDeviceName() const
{
	return m_deviceName;
}

void CScreenDevice::declareDevice(boost::shared_ptr<yApi::IYPluginApi> & api)
{
	if (!api->deviceExists(m_deviceName))
		api->declareDevice(m_deviceName, "Http Screen", "Http Screen");

	if (!api->keywordExists(m_deviceName, m_update_screen_event))
		api->declareKeyword(m_deviceName, m_update_screen_event);
}

void CScreenDevice::updateScreen(const std::string& keyword, const std::string& screenCommands)
{
	//m_update_screen_event->getHistorizer()->set(text);
	if (boost::iequals(m_update_screen_event->getKeyword(), keyword))
	{
		std::vector<CCommand> cmds;
		if (CCommandParser::Parse(screenCommands, cmds))
		{
			m_realScreen->start();
			m_realScreen->update(cmds);
			m_realScreen->finalize();
		}
	}
}