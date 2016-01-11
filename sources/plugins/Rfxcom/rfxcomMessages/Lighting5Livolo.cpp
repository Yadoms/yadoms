#include "stdafx.h"
#include "Lighting5Livolo.h"
#include "RFXtrxHelpers.h"

#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CLighting5Livolo::CLighting5Livolo()
   :m_keyword("state")
{
}

    CLighting5Livolo::~CLighting5Livolo()
    {
    }

std::string CLighting5Livolo::getModel() const
{
   return "Livolo";
}

void CLighting5Livolo::declare(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& deviceName) const
{
   context->declareKeyword(deviceName, m_keyword);
}

void CLighting5Livolo::historize(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& deviceName) const
{
   context->historizeData(deviceName, m_keyword);
}

void CLighting5Livolo::set(const std::string& yadomsCommand)
{
   m_keyword.setCommand(yadomsCommand);
}

void CLighting5Livolo::reset()
{
   m_keyword.set(false);
}

size_t CLighting5Livolo::getMessageNb() const
{
   // To put Livolo module OFF : send a OFF command
   // To put Livolo module ON : send a OFF command, and a toggle command
   return m_keyword.get() ? 2 : 1;
}

void CLighting5Livolo::setFromProtocolState(unsigned char cmdByte, unsigned char /*levelByte*/)
{
   switch (cmdByte)
   {
   case 0x00 :                // Off
   case 0x02 :                // Group Off
      m_keyword.set(false);
       break;
   case 0x01 :                // On
   case 0x03 :                // Group On
      m_keyword.set(true);
       break;
   default:
      {
         BOOST_ASSERT_MSG(false, "Invalid state");
         throw shared::exception::CInvalidParameter(m_keyword.getKeyword());
      }
   }
}

void CLighting5Livolo::toProtocolState(size_t idxMessage, unsigned char& cmdByte, unsigned char& levelByte) const
{
   levelByte = 0;
   if (!m_keyword.get())
   {
      // To put Livolo module OFF : send a OFF command
      cmdByte = 0x00;
   }
   else
   {
      // To put Livolo module ON : send a OFF command, and a toggle command
      cmdByte = idxMessage == 0 ? 0x00 : 0x01;  
   }
}

} // namespace rfxcomMessages