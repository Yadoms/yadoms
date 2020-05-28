#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "HttpController.h"
#include "IRealScreen.h"

class CTftScreen : public IRealScreen
{
public:
	CTftScreen(const boost::shared_ptr<CHttpController> & controller);
	virtual ~CTftScreen();

	void setup_ip(const std::string& ip) override;
	void start() override;
	void update(const std::vector<CCommand>& cmds) override;
	void finalize() override;

private:
	boost::shared_ptr<CHttpController> m_controller;
	std::string m_ip;
};


