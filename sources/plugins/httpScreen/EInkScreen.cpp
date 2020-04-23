#include "stdafx.h"
#include "EInkScreen.h"



CEInkScreen::CEInkScreen(const boost::shared_ptr<CHttpController>& controller)
   :m_controller(controller)
{

}

CEInkScreen::~CEInkScreen()
{

}

void CEInkScreen::setup_ip(const std::string& ip)
{
   m_ip = ip;
}

void CEInkScreen::start()
{
   m_controller->sendCommand(m_ip, "epdcmd", "SEQ_START");
}

void CEInkScreen::update(const std::vector<CCommand>& cmds)
{
   for (auto cmd : cmds)
   {
      if (cmd.isControl())
      {
         m_controller->sendCommand(m_ip, "epdcmd", cmd.get());
      }
      else
      {
         m_controller->sendCommand(m_ip, "epd", cmd.get());
      }
   }
}

void CEInkScreen::finalize()
{
   m_controller->sendCommand(m_ip, "epdcmd", "SEQ_END");
}
