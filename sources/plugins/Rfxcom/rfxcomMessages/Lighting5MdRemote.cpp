#include "stdafx.h"
#include "Lighting5MdRemote.h"
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CLighting5MdRemote::CLighting5MdRemote(const std::string& model)
      : m_model(model),
        m_keyword(boost::make_shared<yApi::historization::CDimmable>("state"))
   {
   }

   CLighting5MdRemote::~CLighting5MdRemote()
   {
   }

   std::string CLighting5MdRemote::getModel() const
   {
      return m_model;
   }

   boost::shared_ptr<const yApi::historization::IHistorizable> CLighting5MdRemote::keyword() const
   {
      return m_keyword;
   }

   void CLighting5MdRemote::set(const std::string& yadomsCommand)
   {
      m_keyword->set(yadomsCommand);
   }

   void CLighting5MdRemote::reset()
   {
      m_keyword->set(0);
   }

   size_t CLighting5MdRemote::getMessageNb() const
   {
      return 1;
   }

   void CLighting5MdRemote::setFromProtocolState(unsigned char cmdByte,
                                                 unsigned char levelByte)
   {
      switch (cmdByte)
      {
      case 0x00: // Power
         m_keyword->set(0);
         break;
      case 0x01: // Light
         m_keyword->set(100);
         break;
      case 0x04: // 100%
         m_keyword->set(100);
         break;
      case 0x05: // 50%
         m_keyword->set(50);
         break;
      case 0x06: // 25%
         m_keyword->set(25);
         break;
      default:
         throw shared::exception::CInvalidParameter("state, " + boost::lexical_cast<std::string>(cmdByte));
      }
   }

   void CLighting5MdRemote::toProtocolState(size_t /*idxMessage*/,
                                            unsigned char& cmdByte,
                                            unsigned char& levelByte) const
   {
      levelByte = 0;
      if (m_keyword->switchLevel() == 0) cmdByte = 0x00; // Off
      else if (m_keyword->switchLevel() <= 37) cmdByte = 0x06; // 25%
      else if (m_keyword->switchLevel() <= 75) cmdByte = 0x05; // 50%
      else cmdByte = 0x04; // 100%
   }
} // namespace rfxcomMessages
