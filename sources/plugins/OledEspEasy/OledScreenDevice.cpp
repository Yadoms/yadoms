#include "stdafx.h"
#include "OledScreenDevice.h"
#include <shared/http/HttpMethods.h>

COledScreenDevice::COledScreenDevice(boost::shared_ptr<COledEspEasyController> controller, int lineCount)
	:m_controller(controller), m_deviceName("oledssd1306"), m_lineCount(lineCount)
{
	
}

COledScreenDevice::COledScreenDevice(boost::shared_ptr<COledEspEasyController> controller, std::string deviceName, int lineCount)
	: m_controller(controller), m_deviceName(std::move(deviceName)), m_lineCount(lineCount)
{
	
}


COledScreenDevice::~COledScreenDevice()
{
	
}

const std::string& COledScreenDevice::getDeviceName() const
{
	return m_deviceName;
}

void COledScreenDevice::declareDevice(boost::shared_ptr<yApi::IYPluginApi> & api)
{
	if (!api->deviceExists(m_deviceName))
		api->declareDevice(m_deviceName, "SSD1306", "OLED SSD1306 Display");

	for (int i = 0; i < m_lineCount; ++i)
	{
		auto lineI = boost::make_shared<COledScreenLine>(i+1);
		if (!api->keywordExists(m_deviceName, lineI->getHistorizer()))
			api->declareKeyword(m_deviceName, lineI->getHistorizer());
		m_keywordLines.push_back(lineI);
	}

	m_keywordControl = boost::make_shared<COledScreenControl>();

	if (!api->keywordExists(m_deviceName, m_keywordControl->get_on_event()))
		api->declareKeyword(m_deviceName, m_keywordControl->get_on_event());
	if (!api->keywordExists(m_deviceName, m_keywordControl->get_off_event()))
		api->declareKeyword(m_deviceName, m_keywordControl->get_off_event());
	if (!api->keywordExists(m_deviceName, m_keywordControl->get_clear_event()))
		api->declareKeyword(m_deviceName, m_keywordControl->get_clear_event());

}

void COledScreenDevice::updateScreen(const std::string& keyword, const std::string& text)
{
	const auto keywordIt = std::find_if(m_keywordLines.begin(), m_keywordLines.end(), [&keyword](const boost::shared_ptr<COledScreenLine>& x) { return x->getKeywordName() == keyword; });
	if(keywordIt != m_keywordLines.end())
	{
		m_controller->update_line((*keywordIt)->getLine(), 1, text);
		(*keywordIt)->getHistorizer()->set(text);
	}
	else
	{
		if(boost::iequals(m_keywordControl->get_on_event()->getKeyword(), keyword))
		{
			m_controller->switch_on();	
		}
		else if (boost::iequals(m_keywordControl->get_off_event()->getKeyword(), keyword))
		{
			m_controller->switch_off();
		}
		else if (boost::iequals(m_keywordControl->get_clear_event()->getKeyword(), keyword))
		{
			m_controller->clear_screen();
		}
	}

}