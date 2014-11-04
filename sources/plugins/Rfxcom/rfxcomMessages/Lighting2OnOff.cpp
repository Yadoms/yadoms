#include "stdafx.h"
#include "Lighting2OnOff.h"
#include "RFXtrxHelpers.h"

#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CLighting2OnOff::CLighting2OnOff(const std::string& model)
   :m_keyword("state"), m_model(model)
{
}

std::string CLighting2OnOff::getModel() const
{
   return m_model;
}

void CLighting2OnOff::declare(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const
{
   context->declareKeyword(deviceName, m_keyword);
}

void CLighting2OnOff::historize(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const
{
   context->historizeData(deviceName, m_keyword);
}

void CLighting2OnOff::set(const shared::CDataContainer& yadomsCommand)
{
   m_keyword.set(yadomsCommand);
}

void CLighting2OnOff::default()
{
   m_keyword.set(false);
}

void CLighting2OnOff::idFromProtocol(unsigned char id1Byte, unsigned char id2Byte, unsigned char id3Byte, unsigned char id4Byte, unsigned char& houseCode, unsigned int& id) const
{
   houseCode = id1Byte;
   id = id2Byte << 16 | id3Byte << 8 | id4Byte;
}

void CLighting2OnOff::idToProtocol(unsigned char houseCode, unsigned int id, unsigned char& id1Byte, unsigned char& id2Byte, unsigned char& id3Byte, unsigned char& id4Byte) const
{
   id1Byte = houseCode;
   id2Byte = (unsigned char) (0xFF & (id >> 16));
   id3Byte = (unsigned char) (0xFF & (id >> 8));
   id4Byte = (unsigned char) (0xFF & id);
}

void CLighting2OnOff::setFromProtocolState(unsigned char cmdByte, unsigned char /*levelByte*/)
{
   switch(cmdByte)
   {
   case light2_sOn: m_keyword.set(true);
   case light2_sOff: m_keyword.set(false);
   default:
      {
         BOOST_ASSERT_MSG(false, "Invalid state");
         throw shared::exception::CInvalidParameter("state");
      }
   }
}

void CLighting2OnOff::toProtocolState(unsigned char& cmdByte, unsigned char& levelByte) const
{
   cmdByte = m_keyword.get() ? light2_sOn : light2_sOff;
   levelByte = 0;
}

} // namespace rfxcomMessages