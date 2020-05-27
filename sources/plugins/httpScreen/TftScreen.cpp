#include "stdafx.h"
#include "TftScreen.h"



CTftScreen::CTftScreen(const boost::shared_ptr<CHttpController>& controller)
   :m_controller(controller)
{

}

CTftScreen::~CTftScreen()
{

}

void CTftScreen::setup_ip(const std::string& ip)
{
   m_ip = ip;
}

void CTftScreen::start()
{

}

void CTftScreen::update(const std::vector<CCommand>& cmds)
{
   for (auto cmd : cmds)
   {
      if (cmd.isControl())
      {
         m_controller->sendCommand(m_ip, "oledcmd", cmd.get());
      }
      else
      {
         m_controller->sendCommand(m_ip, "oled", cmd.get());
      }
   }
}

void CTftScreen::finalize()
{

}
