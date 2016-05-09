#include "stdafx.h"
#include "Lighting5Livolo.h"

#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CLighting5Livolo::CLighting5Livolo()
      : m_keyword("state")
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
      case light5_sLivoloAllOff:
         m_keyword.set(false);
         break;
      default:
         // We can not deal with toogle commands because if a message is lots, state seen by Yadoms can be inverted from device state
         throw shared::exception::CInvalidParameter("state, " + boost::lexical_cast<std::string>(cmdByte));
      }
   }

   void CLighting5Livolo::toProtocolState(size_t idxMessage, unsigned char& cmdByte, unsigned char& levelByte) const
   {
      levelByte = 0;
      if (!m_keyword.get())
      {
         // To put Livolo module OFF : send a OFF command
         cmdByte = light5_sLivoloAllOff;
      }
      else
      {
         // To put Livolo module ON : send a OFF command, and a toggle command
         cmdByte = idxMessage == 0 ? light5_sLivoloAllOff : light5_sLivoloGang1Toggle;
      }
   }
} // namespace rfxcomMessages

