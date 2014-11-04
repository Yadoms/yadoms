#include "stdafx.h"
#include "Lighting5OnOffKeyword.h"
#include "RFXtrxHelpers.h"

#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CLighting5OnOffKeyword::CLighting5OnOffKeyword(const std::string& model)
   :m_keyword("state"), m_model(model)
{
}

std::string CLighting5OnOffKeyword::getModel() const
{
   return m_model;
}

void CLighting5OnOffKeyword::declare(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const
{
   context->declareKeyword(deviceName, m_keyword);
}

void CLighting5OnOffKeyword::historize(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const
{
   context->historizeData(deviceName, m_keyword);
}

void CLighting5OnOffKeyword::set(const shared::CDataContainer& yadomsCommand)
{
   m_keyword.set(yadomsCommand);
}

void CLighting5OnOffKeyword::default()
{
   m_keyword.set(false);
}

void CLighting5OnOffKeyword::setFromProtocolState(unsigned char cmdByte, unsigned char /*levelByte*/)
{
   switch (cmdByte)
   {
   case 0x00 :                // Off
   case 0x02 :                // Group Off
      m_keyword.set(false);
   case 0x01 :                // On
   case 0x03 :                // Group On
      m_keyword.set(true);
   default:
      {
         BOOST_ASSERT_MSG(false, "Invalid state");
         throw shared::exception::CInvalidParameter(m_keyword.getKeyword());
      }
   }
}

void CLighting5OnOffKeyword::toProtocolState(unsigned char& cmdByte, unsigned char& levelByte) const
{
   levelByte = 0;
   cmdByte = m_keyword.get() ? 0x01 : 0x00;
}

} // namespace rfxcomMessages