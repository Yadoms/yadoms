#include "stdafx.h"
#include "X10Basic.h"
#include "../rfxcomMessages/RFXtrxDefinitions.h"
#include "../rfxcomMessages/Lighting1.h"
#include "../rfxcomMessages/Lighting3.h"
#include "../rfxcomMessages/Curtain1.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/xpl/XplException.h>
#include <shared/StringExtension.h>

namespace xplMessages
{

CXplMsgX10Basic::CXplMsgX10Basic(const shared::xpl::CXplMessage& xplMessage, boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider)
   :m_xplMessage(xplMessage), m_seqNumberProvider(seqNumberProvider)
{
   BOOST_ASSERT_MSG(m_xplMessage.getMessageSchemaIdentifier().getClassId() == "x10" &&
      m_xplMessage.getMessageSchemaIdentifier().getTypeId() == "basic", "Wrong message format");
}

CXplMsgX10Basic::~CXplMsgX10Basic()
{
}

boost::shared_ptr<rfxcomMessages::IRfxcomMessage> CXplMsgX10Basic::toRfxComMessage() const
{
   //TODO déplacer le code dans le ctor (permet d'appeler plusieurs fois cette fonction sans refaire la conversion) ?

   if (!m_xplMessage.hasBodyValue("protocol"))
      return createLighting1Msg();   // If "protocol" is not present, protocol is considered as x10

   const std::string& protocol = m_xplMessage.getBodyValue("protocol", "x10");
   if (protocol == "arc" || protocol == "flamingo" || protocol == "waveman")
      return createLighting1Msg();

   if (protocol == "koppla")
      return createLighting3Msg();

   if (protocol == "harrison")
      return createCurtain1Msg();

   throw shared::exception::CInvalidParameter("Xpl message type");
}

boost::shared_ptr<rfxcomMessages::IRfxcomMessage> CXplMsgX10Basic::createLighting1Msg() const
{
   // subType
   unsigned char subType;
   if (!m_xplMessage.hasBodyValue("protocol"))
      subType = sTypeX10;
   else
   {
      const std::string& protocol = m_xplMessage.getBodyValue("protocol");
      if (protocol == "arc")
         subType = sTypeARC;
      if (protocol == "famingo")
         subType = sTypeAB400D;
      if (protocol == "waveman")
         subType = sTypeWaveman;
      else
      {
         BOOST_ASSERT_MSG(false, "Fail to extract command data from XPL message : unknown protocol value");
         throw shared::xpl::CXplException("Fail to extract command data from XPL message : unknown protocol value " + protocol);
      }
   }

   // houseCode and unitCode
   std::string device = m_xplMessage.getBodyValue("device");

   unsigned char houseCode = device[0];   // houseCode is the first character of the XPL device data

   unsigned char unitCode = 1;
   if (device.size() > 1)
   {
      // unitCode is the rest of the XPL device data string, converted to hex
      try
      {
         unitCode = boost::lexical_cast<unsigned char>(device.substr(1, std::string::npos));
      }
      catch (boost::bad_lexical_cast& e)
      {
         throw shared::xpl::CXplException(std::string("Fail to extract device data from XPL message : ") + e.what());
      }
   }

   // cmnd
   unsigned char cmnd;
   std::string command = m_xplMessage.getBodyValue("command");
   if (command == "on")
      cmnd = light1_sOn;
   else if (command == "off")
      cmnd = light1_sOff;
   else if (command == "dim")
      cmnd = light1_sDim;
   else if (command == "bright")
      cmnd = light1_sBright;
   else if (command == "all_lights_on")
      cmnd = light1_sAllOn;
   else if (command == "all_lights_off")
      cmnd = light1_sAllOff;
   else
      throw shared::xpl::CXplException("Fail to extract command data from XPL message : unknown command value " + command);

   boost::shared_ptr<rfxcomMessages::IRfxcomMessage> rfxcomMsg (new rfxcomMessages::CLighting1(subType, houseCode, unitCode, cmnd, m_seqNumberProvider));
   return rfxcomMsg;
}

boost::shared_ptr<rfxcomMessages::IRfxcomMessage> CXplMsgX10Basic::createLighting3Msg() const
{
   BOOST_ASSERT_MSG(m_xplMessage.getBodyValue("protocol") == "koppla", "This XPL message is not supported by Lighting3 RFXCom message");

   // system and channel
   std::string device = m_xplMessage.getBodyValue("device");

   unsigned char system = (unsigned char)(toupper(device[0])) - 'A';   // system is the first character of the XPL device data, to hex

   // channel is the rest of the XPL device data string, converted to hex
   unsigned char channel;
   try
   {
      channel = boost::lexical_cast<unsigned char>(device.substr(1, std::string::npos));
   }
   catch (boost::bad_lexical_cast& e)
   {
      throw shared::xpl::CXplException(std::string("Fail to extract device data from XPL message : ") + e.what());
   }

   // cmnd
   unsigned char cmnd;
   std::string command = m_xplMessage.getBodyValue("command");
   if (m_xplMessage.hasBodyValue("level"))
   {
      unsigned char level = boost::lexical_cast<unsigned char>(m_xplMessage.getBodyValue("level")) / 10;
      switch (level)
      {
      case 0: cmnd = light3_sOff; break;
      case 1: cmnd = light3_sLevel1; break;
      case 2: cmnd = light3_sLevel2; break;
      case 3: cmnd = light3_sLevel3; break;
      case 4: cmnd = light3_sLevel4; break;
      case 5: cmnd = light3_sLevel5; break;
      case 6: cmnd = light3_sLevel6; break;
      case 7: cmnd = light3_sLevel7; break;
      case 8: cmnd = light3_sLevel8; break;
      case 9: cmnd = light3_sLevel9; break;
      default: cmnd = light3_sOn; break;
      }
   }
   else
   {
      if (command == "on")
         cmnd = light3_sOn;
      else if (command == "off")
         cmnd = light3_sOff;
      else if (command == "dim")
         cmnd = light3_sDim;
      else if (command == "bright")
         cmnd = light3_sBright;
      else if (command == "all_lights_on")
         cmnd = light1_sAllOn;
      else if (command == "all_lights_off")
         cmnd = light1_sAllOff;
      else
         throw shared::xpl::CXplException("Fail to extract command data from XPL message : unknown command value " + command);
   }

   boost::shared_ptr<rfxcomMessages::IRfxcomMessage> rfxcomMsg (new rfxcomMessages::CLighting3(system, channel, cmnd, m_seqNumberProvider));
   return rfxcomMsg;
}

boost::shared_ptr<rfxcomMessages::IRfxcomMessage> CXplMsgX10Basic::createCurtain1Msg() const
{
   BOOST_ASSERT_MSG(m_xplMessage.getBodyValue("protocol") == "harrison", "This XPL message is not supported by Curtain1 RFXCom message");

   // houseCode and unitCode
   std::string device = m_xplMessage.getBodyValue("device");

   unsigned char houseCode = device[0];   // houseCode is the first character of the XPL device data

   unsigned char unitCode = 1;
   if (device.size() > 1)
   {
      // unitCode is the rest of the XPL device data string, converted to hex
      try
      {
         unitCode = boost::lexical_cast<unsigned char>(device.substr(1, std::string::npos));
      }
      catch (boost::bad_lexical_cast& e)
      {
         throw shared::xpl::CXplException(std::string("Fail to extract device data from XPL message : ") + e.what());
      }
   }

   // cmnd
   unsigned char cmnd;
   std::string command = m_xplMessage.getBodyValue("command");
   if (command == "on")
      cmnd = curtain_sOpen;
   else if (command == "off")
      cmnd = curtain_sClose;
   else if ((command == "dim") || (command == "bright"))
      cmnd = curtain_sStop;
   else if ((command == "all_lights_on") || (command == "all_lights_off"))
      cmnd = curtain_sProgram;
   else
      throw shared::xpl::CXplException("Fail to extract command data from XPL message : unknown command value " + command);

   boost::shared_ptr<rfxcomMessages::IRfxcomMessage> rfxcomMsg (new rfxcomMessages::CCurtain1(houseCode, unitCode, cmnd, m_seqNumberProvider));
   return rfxcomMsg;
}

} // namespace xplMessages