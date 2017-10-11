#include "stdafx.h"
#include "Lighting2OnOff.h"
#include "RFXtrxHelpers.h"
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CLighting2OnOff::CLighting2OnOff(const std::string& model)
      : m_model(model),
        m_keyword(boost::make_shared<yApi::historization::CSwitch>("state"))
   {
   }

   CLighting2OnOff::~CLighting2OnOff()
   {
   }

   std::string CLighting2OnOff::getModel() const
   {
      return m_model;
   }

   boost::shared_ptr<const yApi::historization::IHistorizable> CLighting2OnOff::keyword() const
   {
      return m_keyword;
   }

   void CLighting2OnOff::set(const std::string& yadomsCommand)
   {
      m_keyword->setCommand(yadomsCommand);
   }

   void CLighting2OnOff::reset()
   {
      m_keyword->set(false);
   }

   void CLighting2OnOff::idFromProtocol(unsigned char id1Byte,
                                        unsigned char id2Byte,
                                        unsigned char id3Byte,
                                        unsigned char id4Byte,
                                        unsigned char& houseCode,
                                        unsigned int& id) const
   {
      houseCode = id1Byte;
      id = id2Byte << 16 | id3Byte << 8 | id4Byte;
   }

   void CLighting2OnOff::idToProtocol(unsigned char houseCode,
                                      unsigned int id,
                                      unsigned char& id1Byte,
                                      unsigned char& id2Byte,
                                      unsigned char& id3Byte,
                                      unsigned char& id4Byte) const
   {
      id1Byte = houseCode;
      id2Byte = static_cast<unsigned char>(0xFF & (id >> 16));
      id3Byte = static_cast<unsigned char>(0xFF & (id >> 8));
      id4Byte = static_cast<unsigned char>(0xFF & id);
   }

   void CLighting2OnOff::setFromProtocolState(unsigned char cmdByte,
                                              unsigned char /*levelByte*/)
   {
      switch (cmdByte)
      {
      case light2_sOn: m_keyword->set(true);
         break;
      case light2_sOff: m_keyword->set(false);
         break;
      default:
         throw shared::exception::CInvalidParameter("state, " + boost::lexical_cast<std::string>(cmdByte));
      }
   }

   void CLighting2OnOff::toProtocolState(unsigned char& cmdByte,
                                         unsigned char& levelByte) const
   {
      cmdByte = m_keyword->get() ? light2_sOn : light2_sOff;
      levelByte = 0;
   }
} // namespace rfxcomMessages
