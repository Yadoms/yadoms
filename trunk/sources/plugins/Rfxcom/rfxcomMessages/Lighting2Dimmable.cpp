#include "stdafx.h"
#include "Lighting2Dimmable.h"
#include "RFXtrxHelpers.h"

#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CLighting2Dimmable::CLighting2Dimmable(const std::string& model)
   :m_keyword("state"), m_model(model)
{
}

std::string CLighting2Dimmable::getModel() const
{
   return m_model;
}

void CLighting2Dimmable::declare(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const
{
   context->declareKeyword(deviceName, m_keyword);
}

void CLighting2Dimmable::historize(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const
{
   context->historizeData(deviceName, m_keyword);
}

void CLighting2Dimmable::set(const shared::CDataContainer& yadomsCommand)
{
   m_keyword.set(yadomsCommand);
}

void CLighting2Dimmable::default()
{
   m_keyword.set(0);
}

void CLighting2Dimmable::idFromProtocol(unsigned char id1Byte, unsigned char id2Byte, unsigned char id3Byte, unsigned char id4Byte, unsigned char& houseCode, unsigned int& id) const
{
   houseCode = 0;
   id = id1Byte << 24 | id2Byte << 16 | id3Byte << 8 | id4Byte;
}

void CLighting2Dimmable::idToProtocol(unsigned char /*houseCode*/, unsigned int id, unsigned char& id1Byte, unsigned char& id2Byte, unsigned char& id3Byte, unsigned char& id4Byte) const
{
   id1Byte = (unsigned char) (0xFF & (id >> 24));
   id2Byte = (unsigned char) (0xFF & (id >> 16));
   id3Byte = (unsigned char) (0xFF & (id >> 8));
   id4Byte = (unsigned char) (0xFF & id);
}

void CLighting2Dimmable::setFromProtocolState(unsigned char cmdByte, unsigned char levelByte)
{
   switch(cmdByte)
   {
   case light2_sOn: m_keyword.set(100); break;
   case light2_sOff: m_keyword.set(0); break;
   case light2_sSetLevel:
      {
         m_keyword.set(levelByte * 100 / 0x0F); // level needs to be from 0 to 100
          break;
      }
   default:
      {
         BOOST_ASSERT_MSG(false, "Invalid state");
         throw shared::exception::CInvalidParameter("state");
      }
   }
}

void CLighting2Dimmable::toProtocolState(unsigned char& cmdByte, unsigned char& levelByte) const
{
   switch(m_keyword.switchLevel())
   {
   case 0 :
      cmdByte = light2_sOff;
      levelByte = 0;
      break;
   case 100 :
      cmdByte = light2_sOn;
      levelByte = 0;
      break;
   default :
      cmdByte = light2_sSetLevel;
      levelByte = (unsigned char) (m_keyword.switchLevel() * 0x0F / 100);   // switchLevel returns value from 0 to 100
      break;
   }
}

} // namespace rfxcomMessages