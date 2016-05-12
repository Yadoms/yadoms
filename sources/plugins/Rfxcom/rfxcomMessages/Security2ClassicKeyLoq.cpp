#include "stdafx.h"
#include "Security2ClassicKeyLoq.h"
#include "RFXtrxHelpers.h"

#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CSecurity2ClassicKeyLoq::CSecurity2ClassicKeyLoq()
   :m_id(0),
   m_button0("button0", yApi::EKeywordAccessMode::kGetSet),
   m_button1("button1", yApi::EKeywordAccessMode::kGetSet),
   m_button2("button2", yApi::EKeywordAccessMode::kGetSet),
   m_button3("button3", yApi::EKeywordAccessMode::kGetSet)
{
}

CSecurity2ClassicKeyLoq::~CSecurity2ClassicKeyLoq()
{
}

std::string CSecurity2ClassicKeyLoq::getModel() const
{
   return "Classic KeyLoq remote";
}

void CSecurity2ClassicKeyLoq::declare(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& deviceName) const
{
   if (!api->keywordExists(deviceName, m_button0))
      api->declareKeyword(deviceName, m_button0);
   if (!api->keywordExists(deviceName, m_button1))
      api->declareKeyword(deviceName, m_button1);
   if (!api->keywordExists(deviceName, m_button2))
      api->declareKeyword(deviceName, m_button2);
   if (!api->keywordExists(deviceName, m_button3))
      api->declareKeyword(deviceName, m_button3);
}

void CSecurity2ClassicKeyLoq::historize(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& deviceName) const
{
   api->historizeData(deviceName, m_button0);
   api->historizeData(deviceName, m_button1);
   api->historizeData(deviceName, m_button2);
   api->historizeData(deviceName, m_button3);
}

void CSecurity2ClassicKeyLoq::setId(unsigned int id)
{
   m_id = id;
}

unsigned int CSecurity2ClassicKeyLoq::getId() const
{
   return m_id;
}

void CSecurity2ClassicKeyLoq::set(const std::string& keyword, const std::string& yadomsCommand)
{
   if (boost::iequals(keyword, m_button0.getKeyword()))
   {
      m_button0.setCommand(yadomsCommand);
      return;
   }
   
   if (boost::iequals(keyword, m_button1.getKeyword()))
   {
      m_button1.setCommand(yadomsCommand);
      return;
   }
   
   if (boost::iequals(keyword, m_button2.getKeyword()))
   {
      m_button2.setCommand(yadomsCommand);
      return;
   }
   
   if (boost::iequals(keyword, m_button3.getKeyword()))
   {
      m_button3.setCommand(yadomsCommand);
      return;
   }

   throw shared::exception::CInvalidParameter("yadomsCommand");
}

void CSecurity2ClassicKeyLoq::resetState()
{
   m_button0.set(false);
   m_button1.set(false);
   m_button2.set(false);
   m_button3.set(false);
}

void CSecurity2ClassicKeyLoq::setFromProtocolState(const RBUF& Security2)
{
   m_id = (Security2.SECURITY2.id8 & 0x0F) << 24
      | Security2.SECURITY2.id7 << 16
      | Security2.SECURITY2.id6 << 8
      | Security2.SECURITY2.id5;

   m_button0.set((Security2.SECURITY2.id8 & 0x10) ? true : false);
   m_button1.set((Security2.SECURITY2.id8 & 0x20) ? true : false);
   m_button2.set((Security2.SECURITY2.id8 & 0x40) ? true : false);
   m_button3.set((Security2.SECURITY2.id8 & 0x80) ? true : false);
}

void CSecurity2ClassicKeyLoq::toProtocolState(RBUF& Security2) const
{
   Security2.SECURITY2.id1 = Security2.SECURITY2.id2 = Security2.SECURITY2.id3 = Security2.SECURITY2.id4 =
      Security2.SECURITY2.id9 = Security2.SECURITY2.id10 = Security2.SECURITY2.id11 = Security2.SECURITY2.id12 =
      Security2.SECURITY2.id13 = Security2.SECURITY2.id14 = Security2.SECURITY2.id15 = Security2.SECURITY2.id16 =
      Security2.SECURITY2.id17 = Security2.SECURITY2.id18 = Security2.SECURITY2.id19 = Security2.SECURITY2.id20 =
      Security2.SECURITY2.id21 = Security2.SECURITY2.id22 = Security2.SECURITY2.id23 = Security2.SECURITY2.id24 = 0;

   Security2.SECURITY2.id5 = static_cast<unsigned char>(m_id & 0xFF);
   Security2.SECURITY2.id6 = static_cast<unsigned char>((m_id & 0xFF00) >> 8);
   Security2.SECURITY2.id7 = static_cast<unsigned char>((m_id & 0xFF0000) >> 16);
   Security2.SECURITY2.id8 = static_cast<unsigned char>((m_id & 0x0F000000) >> 24)
      | (m_button0.get() ? 0x10 : 0)
      | (m_button1.get() ? 0x20 : 0)
      | (m_button2.get() ? 0x40 : 0)
      | (m_button3.get() ? 0x80 : 0);
}

} // namespace rfxcomMessages