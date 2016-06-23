#include "stdafx.h"
#include "Lighting5LightwaveRf.h"
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CLighting5LightwaveRf::CLighting5LightwaveRf()
      : m_keyword(boost::make_shared<yApi::historization::CDimmable>("state"))
   {
   }

   CLighting5LightwaveRf::~CLighting5LightwaveRf()
   {
   }

   std::string CLighting5LightwaveRf::getModel() const
   {
      return "LightwaveRF, Siemens (AD protocol)";
   }

   boost::shared_ptr<const yApi::historization::IHistorizable> CLighting5LightwaveRf::keyword() const
   {
      return m_keyword;
   }

   void CLighting5LightwaveRf::set(const std::string& yadomsCommand)
   {
      m_keyword->set(yadomsCommand);
   }

   void CLighting5LightwaveRf::reset()
   {
      m_keyword->set(0);
   }

   size_t CLighting5LightwaveRf::getMessageNb() const
   {
      return 1;
   }

   void CLighting5LightwaveRf::setFromProtocolState(unsigned char cmdByte,
                                                    unsigned char levelByte)
   {
      switch (cmdByte)
      {
      case 0x00: // Off
         m_keyword->set(0);
         break;
      case 0x01: // On
         m_keyword->set(100);
         break;
      case 0x10: // Set level
         m_keyword->set(levelByte * 100 / 0x1F);
         break;
      default:
         throw shared::exception::CInvalidParameter("state, " + boost::lexical_cast<std::string>(cmdByte));
      }
   }

   void CLighting5LightwaveRf::toProtocolState(size_t /*idxMessage*/,
                                               unsigned char& cmdByte,
                                               unsigned char& levelByte) const
   {
      if (m_keyword->switchLevel() == 0)
      {
         cmdByte = 0x00;
         levelByte = 0;
      }
      else if (m_keyword->switchLevel() == 100)
      {
         cmdByte = 0x01;
         levelByte = 0;
      }
      else
      {
         cmdByte = 0x10;
         levelByte = static_cast<unsigned char>(m_keyword->switchLevel() * 0x1F / 100);
      }
   }
} // namespace rfxcomMessages


