#include "stdafx.h"
#include "Lighting5MdRemote.h"
#include "RFXtrxHelpers.h"

#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CLighting5MdRemoteKeyword::CLighting5MdRemoteKeyword()
   :m_keyword("state")
{
}

std::string CLighting5MdRemoteKeyword::getModel() const
{
   return "MDREMOTE LED dimmer";
}

void CLighting5MdRemoteKeyword::declare(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const
{
   context->declareKeyword(deviceName, m_keyword);
}

void CLighting5MdRemoteKeyword::historize(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const
{
   context->historizeData(deviceName, m_keyword);
}

void CLighting5MdRemoteKeyword::set(const shared::CDataContainer& yadomsCommand)
{
   m_keyword.set(yadomsCommand);
}

void CLighting5MdRemoteKeyword::default()
{
   m_keyword.set(0);
}

size_t CLighting5MdRemoteKeyword::getMessageNb() const
{
   return 1;
}

void CLighting5MdRemoteKeyword::setFromProtocolState(unsigned char cmdByte, unsigned char levelByte)
{
   switch (cmdByte)
   {
   case 0x00 :                // Power
      m_keyword.set(0);
       break;
   case 0x01 :                // Light
      m_keyword.set(100);
       break;
   case 0x04 :                // 100%
      m_keyword.set(100);
       break;
   case 0x05 :                // 50%
      m_keyword.set(50);
       break;
   case 0x06 :                // 25%
      m_keyword.set(25);
       break;
   default:
      {
         BOOST_ASSERT_MSG(false, "Invalid state");
         throw shared::exception::CInvalidParameter(m_keyword.getKeyword());
      }
   }
}

void CLighting5MdRemoteKeyword::toProtocolState(size_t /*idxMessage*/, unsigned char& cmdByte, unsigned char& levelByte) const
{
   levelByte = 0;
   if      (m_keyword.switchLevel() ==   0) cmdByte = 0x00; // Off
   else if (m_keyword.switchLevel() <= 37 ) cmdByte = 0x06; // 25%
   else if (m_keyword.switchLevel() <= 75 ) cmdByte = 0x05; // 50%
   else                                     cmdByte = 0x04; // 100%
}

} // namespace rfxcomMessages