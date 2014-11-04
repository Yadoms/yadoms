#include "stdafx.h"
#include "Lighting5LightwaveRfKeyword.h"
#include "RFXtrxHelpers.h"

#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CLighting5LightwaveRfKeyword::CLighting5LightwaveRfKeyword()
   :m_keyword("state")
{
}

std::string CLighting5LightwaveRfKeyword::getModel() const
{
   return "LightwaveRF, Siemens (AD protocol)";
}

void CLighting5LightwaveRfKeyword::declare(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const
{
   context->declareKeyword(deviceName, m_keyword);
}

void CLighting5LightwaveRfKeyword::historize(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const
{
   context->historizeData(deviceName, m_keyword);
}

void CLighting5LightwaveRfKeyword::set(const shared::CDataContainer& yadomsCommand)
{
   m_keyword.set(yadomsCommand);
}

void CLighting5LightwaveRfKeyword::default()
{
   m_keyword.set(0);
}

void CLighting5LightwaveRfKeyword::setFromProtocolState(unsigned char cmdByte, unsigned char levelByte)
{
   switch (cmdByte)
   {
   case 0x00 :                // Off
      m_keyword.set(0);
   case 0x01 :                // On
      m_keyword.set(100);
   case 0x10 :                // Set level
      m_keyword.set(levelByte * 100 / 0x1F);
   default:
      {
         BOOST_ASSERT_MSG(false, "Invalid state");
         throw shared::exception::CInvalidParameter(m_keyword.getKeyword());
      }
   }
}

void CLighting5LightwaveRfKeyword::toProtocolState(unsigned char& cmdByte, unsigned char& levelByte) const
{
   if      (m_keyword.switchLevel() ==   0) { cmdByte = 0x00; levelByte = 0; }
   else if (m_keyword.switchLevel() == 100) { cmdByte = 0x01; levelByte = 0; }
   else                                     { cmdByte = 0x10; levelByte = (unsigned char) (m_keyword.switchLevel() * 0x1F / 100); }
}

} // namespace rfxcomMessages