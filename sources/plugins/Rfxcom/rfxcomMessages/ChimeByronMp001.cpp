#include "stdafx.h"
#include "ChimeByronMp001.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CChimeByronMp001::CChimeByronMp001()
      : m_event("event")
   {
   }

   std::string CChimeByronMp001::getModel() const
   {
      return "Byron MP001";
   }

   void CChimeByronMp001::declare(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& deviceName) const
   {
      api->declareKeyword(deviceName, m_event);
   }

   void CChimeByronMp001::historize(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& deviceName) const
   {
      api->historizeData(deviceName, m_event);
   }

   void CChimeByronMp001::set(const std::string& /*yadomsCommand*/, const shared::CDataContainer& /*deviceDetails*/)
   {
   }

   void CChimeByronMp001::reset()
   {
   }

   unsigned int CChimeByronMp001::idFromProtocol(unsigned char id1, unsigned char id2, unsigned char sound)
   {
      return sound << 16 | id1 << 8 | id2;
   }

   void CChimeByronMp001::idToProtocol(unsigned int id, unsigned char& id1, unsigned char& id2, unsigned char& sound) const
   {
      sound = (unsigned char) (0xFF & (id >> 16));
      id1 = (unsigned char) (0xFF & (id >> 8));
      id2 = (unsigned char) (0xFF & id);
   }

   void CChimeByronMp001::setFromProtocolState(unsigned char cmd)
   {
      // No data
   }

   void CChimeByronMp001::toProtocolState(unsigned char& sound) const
   {
      // Nothing to do, sound is used à ID3
   }
} // namespace rfxcomMessages


