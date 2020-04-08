#include "stdafx.h"
#include "ChimeByronMp001.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CChimeByronMp001::CChimeByronMp001(const std::string& model)
      : m_model(model),
        m_event(boost::make_shared<shared::plugin::yPluginApi::historization::CEvent>("event")),
        m_keywords({m_event})
   {
   }

   std::string CChimeByronMp001::getModel() const
   {
      return m_model;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CChimeByronMp001::keywords() const
   {
      return m_keywords;
   }

   void CChimeByronMp001::set(const std::string& /*yadomsCommand*/, const shared::CDataContainerSharedPtr& /*deviceDetails*/)
   {
   }

   void CChimeByronMp001::reset()
   {
   }

   unsigned int CChimeByronMp001::idFromProtocol(unsigned char id1,
                                                 unsigned char id2,
                                                 unsigned char sound)
   {
      return sound << 16 | id1 << 8 | id2;
   }

   void CChimeByronMp001::idToProtocol(unsigned int id,
                                       unsigned char& id1,
                                       unsigned char& id2,
                                       unsigned char& sound) const
   {
      sound = static_cast<unsigned char>(0xFF & (id >> 16));
      id1 = static_cast<unsigned char>(0xFF & (id >> 8));
      id2 = static_cast<unsigned char>(0xFF & id);
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
