#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "HttpController.h"
#include "IRealScreen.h"

class CEInkScreen : public IRealScreen
{
public:
	CEInkScreen(const boost::shared_ptr<CHttpController>& controller);
	virtual ~CEInkScreen();

	void setup_ip(const std::string& ip) override;
	void start() override;
	void update(const std::vector<CCommand>& cmds) override;
	void finalize() override;

private:
	boost::shared_ptr<CHttpController> m_controller;
	std::string m_ip;
};


