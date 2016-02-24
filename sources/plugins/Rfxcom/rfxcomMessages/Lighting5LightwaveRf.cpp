#include "stdafx.h"
#include "Lighting5LightwaveRf.h"
#include "RFXtrxHelpers.h"

#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CLighting5LightwaveRfKeyword::CLighting5LightwaveRfKeyword()
   :m_keyword("state")
{
}

CLighting5LightwaveRfKeyword::~CLighting5LightwaveRfKeyword()
{
}
    
std::string CLighting5LightwaveRfKeyword::getModel() const
{
   return "LightwaveRF, Siemens (AD protocol)";
}

void CLighting5LightwaveRfKeyword::declare(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& deviceName) const
{
   context->declareKeyword(deviceName, m_keyword);
}

void CLighting5LightwaveRfKeyword::historize(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& deviceName) const
{
   context->historizeData(deviceName, m_keyword);
}

void CLighting5LightwaveRfKeyword::set(const std::string& yadomsCommand)
{
   m_keyword.set(yadomsCommand);
}

void CLighting5LightwaveRfKeyword::reset()
{
   m_keyword.set(0);
}

size_t CLighting5LightwaveRfKeyword::getMessageNb() const
{
   return 1;
}

void CLighting5LightwaveRfKeyword::setFromProtocolState(unsigned char cmdByte, unsigned char levelByte)
{
   switch (cmdByte)
   {
   case 0x00 :                // Off
      m_keyword.set(0);
       break;
   case 0x01 :                // On
      m_keyword.set(100);
       break;
   case 0x10 :                // Set level
      m_keyword.set(levelByte * 100 / 0x1F);
       break;
   default:
         throw shared::exception::CInvalidParameter("state, " + boost::lexical_cast<std::string>(cmdByte));
   }
}

void CLighting5LightwaveRfKeyword::toProtocolState(size_t /*idxMessage*/, unsigned char& cmdByte, unsigned char& levelByte) const
{
   if      (m_keyword.switchLevel() ==   0) { cmdByte = 0x00; levelByte = 0; }
   else if (m_keyword.switchLevel() == 100) { cmdByte = 0x01; levelByte = 0; }
   else                                     { cmdByte = 0x10; levelByte = (unsigned char) (m_keyword.switchLevel() * 0x1F / 100); }
}

} // namespace rfxcomMessages