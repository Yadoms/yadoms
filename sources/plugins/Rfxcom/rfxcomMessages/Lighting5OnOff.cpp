#include "stdafx.h"
#include "Lighting5OnOff.h"
#include "RFXtrxHelpers.h"

#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CLighting5OnOff::CLighting5OnOff(const std::string& model)
   :m_model(model), m_keyword("state")
{
}
    
    CLighting5OnOff::~CLighting5OnOff()
    {
    }
    
std::string CLighting5OnOff::getModel() const
{
   return m_model;
}

void CLighting5OnOff::declare(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const
{
   context->declareKeyword(deviceName, m_keyword);
}

void CLighting5OnOff::historize(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const
{
   context->historizeData(deviceName, m_keyword);
}

void CLighting5OnOff::set(const std::string& yadomsCommand)
{
   m_keyword.set(yadomsCommand);
}

void CLighting5OnOff::reset()
{
   m_keyword.set(false);
}

size_t CLighting5OnOff::getMessageNb() const
{
   return 1;
}

void CLighting5OnOff::setFromProtocolState(unsigned char cmdByte, unsigned char /*levelByte*/)
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

void CLighting5OnOff::toProtocolState(size_t /*idxMessage*/, unsigned char& cmdByte, unsigned char& levelByte) const
{
   levelByte = 0;
   cmdByte = m_keyword.get() ? 0x01 : 0x00;
}

} // namespace rfxcomMessages