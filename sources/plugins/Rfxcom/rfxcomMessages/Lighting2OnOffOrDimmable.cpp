#include "stdafx.h"
#include "Lighting2OnOffOrDimmable.h"
#include "RFXtrxHelpers.h"
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CLighting2OnOffOrDimmable::CLighting2OnOffOrDimmable(const std::string& model,
      EDeviceType deviceType)
      : m_model(model),
        m_state(boost::make_shared<yApi::historization::CSwitch>("state")),
        m_level(boost::make_shared<yApi::historization::CDimmable>("level"))
   {
      switch(deviceType)
      {
      case kOnOff:
         m_keywords.push_back(m_state);
         break;
      case kDimmable:
         m_keywords.push_back(m_level);
         break;
      default:
         // Will be autodetect
         break;
      }
   }

   CLighting2OnOffOrDimmable::~CLighting2OnOffOrDimmable()
   {
   }

   std::string CLighting2OnOffOrDimmable::getModel() const
   {
      return m_model;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CLighting2OnOffOrDimmable::keywords() const
   {
      return m_keywords;
   }

   void CLighting2OnOffOrDimmable::set(boost::shared_ptr<const yApi::IDeviceCommand> yadomsCommand)
   {
      if (yadomsCommand->getKeyword() == m_state->getKeyword())
      {
         m_state->setCommand(yadomsCommand->getBody());
         m_keywords.push_back(m_state);
      }
      else if (yadomsCommand->getKeyword() == m_level->getKeyword())
      {
         m_level->set(yadomsCommand->getBody());
         m_keywords.push_back(m_level);
      }
   }

   void CLighting2OnOffOrDimmable::reset()
   {
      m_state->set(false);
      m_level->set(0);
   }

   void CLighting2OnOffOrDimmable::idFromProtocol(unsigned char id1Byte,
                                           unsigned char id2Byte,
                                           unsigned char id3Byte,
                                           unsigned char id4Byte,
                                           unsigned char& houseCode,
                                           unsigned int& id) const
   {
      houseCode = 0;
      id = id1Byte << 24 | id2Byte << 16 | id3Byte << 8 | id4Byte;
   }

   void CLighting2OnOffOrDimmable::idToProtocol(unsigned char /*houseCode*/,
                                         unsigned int id,
                                         unsigned char& id1Byte,
                                         unsigned char& id2Byte,
                                         unsigned char& id3Byte,
                                         unsigned char& id4Byte) const
   {
      id1Byte = static_cast<unsigned char>(0xFF & (id >> 24));
      id2Byte = static_cast<unsigned char>(0xFF & (id >> 16));
      id3Byte = static_cast<unsigned char>(0xFF & (id >> 8));
      id4Byte = static_cast<unsigned char>(0xFF & id);
   }

   void CLighting2OnOffOrDimmable::setFromProtocolState(unsigned char cmdByte,
                                                 unsigned char levelByte)
   {
      switch (cmdByte)
      {
      case light2_sOn:
         m_state->set(true);
         m_keywords.push_back(m_state);
         break;
      case light2_sOff:
         m_state->set(false);
         m_keywords.push_back(m_state);
         break;
      case light2_sSetLevel:
         {
            m_level->set(levelByte * 100 / 0x0F); // level needs to be from 0 to 100
            m_keywords.push_back(m_level);
            break;
         }
      default:
         {
            throw shared::exception::CInvalidParameter("state, " + boost::lexical_cast<std::string>(cmdByte));
         }
      }
   }

   void CLighting2OnOffOrDimmable::toProtocolState(unsigned char& cmdByte,
                                            unsigned char& levelByte) const
   {
      if (std::find(m_keywords.begin(), m_keywords.end(), m_level) != m_keywords.end())
      {
         cmdByte = light2_sSetLevel;
         levelByte = static_cast<unsigned char>(m_level->switchLevel() * 0x0F / 100); // switchLevel returns value from 0 to 100
         return;
      }

      if (std::find(m_keywords.begin(), m_keywords.end(), m_state) != m_keywords.end())
      {
         cmdByte = m_state->get() ? light2_sOn : light2_sOff;
         levelByte = 0;
         return;
      }

      throw std::invalid_argument("No keyword for CLighting2OnOffOrDimmable::toProtocolState");
   }
} // namespace rfxcomMessages
