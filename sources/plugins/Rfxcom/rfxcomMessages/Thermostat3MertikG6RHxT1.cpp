#include "stdafx.h"
#include "Thermostat3MertikG6RHxT1.h"
#include "RFXtrxHelpers.h"
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CThermostat3MertikG6RHxT1::CThermostat3MertikG6RHxT1(unsigned char subType)
      : m_subType(subType),
      m_statusByte(0),
      m_onOff(boost::make_shared<yApi::historization::CSwitch>("onOff")),
      m_upDown(boost::make_shared<yApi::historization::CUpDownStop>("upDown", yApi::EKeywordAccessMode::kGetSet)),
      m_runUpDown(boost::make_shared<yApi::historization::CUpDownStop>("runUpDown", yApi::EKeywordAccessMode::kGetSet)),
      m_keywords({ m_onOff, m_upDown, m_runUpDown })
   {
   }

   CThermostat3MertikG6RHxT1::~CThermostat3MertikG6RHxT1()
   {
   }

   std::string CThermostat3MertikG6RHxT1::getModel() const
   {
      switch(m_subType)
      {
      case sTypeMertikG6RH3T1:
         return "Mertik G6R-H3T1";
      case sTypeMertikG6RH4T1:
         return "Mertik G6R-H4T1";
      default:
         return "Unknown";
      }
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& CThermostat3MertikG6RHxT1::keywords() const
   {
      return m_keywords;
   }

   void CThermostat3MertikG6RHxT1::set(const std::string& keyword, const std::string& yadomsCommand)
   {
      if (boost::iequals(keyword, m_onOff->getKeyword()))
      {
         m_onOff->setCommand(yadomsCommand);
         m_statusByte = m_onOff.get() ? thermostat3_sOn : thermostat3_sOff;
      }
      else if (boost::iequals(keyword, m_upDown->getKeyword()))
      {
         m_upDown->setCommand(yadomsCommand);
         switch (m_upDown->get())
         {
         case yApi::historization::EUpDownStopCommand::kUpValue: m_statusByte = thermostat3_sUp;
            break;
         case yApi::historization::EUpDownStopCommand::kDownValue: m_statusByte = thermostat3_sDown;
            break;
         case yApi::historization::EUpDownStopCommand::kStopValue: m_statusByte = thermostat3_sStop;
            break;
         default: throw shared::exception::CInvalidParameter("yadomsCommand");
         }
      }
      else if (boost::iequals(keyword, m_runUpDown->getKeyword()))
      {
         m_runUpDown->setCommand(yadomsCommand);
         switch (m_runUpDown->get())
         {
         case yApi::historization::EUpDownStopCommand::kUpValue: m_statusByte = thermostat3_sRunUp;
            break;
         case yApi::historization::EUpDownStopCommand::kDownValue: m_statusByte = thermostat3_sRunDown;
            break;
         case yApi::historization::EUpDownStopCommand::kStopValue: m_statusByte = thermostat3_sStop;
            break;
         default: throw shared::exception::CInvalidParameter("yadomsCommand");
         }
      }
   }

   void CThermostat3MertikG6RHxT1::reset()
   {
      m_onOff->set(false);
      m_upDown->set(yApi::historization::EUpDownStopCommand::kStop);
      m_runUpDown->set(yApi::historization::EUpDownStopCommand::kStop);
   }

   void CThermostat3MertikG6RHxT1::setFromProtocolState(unsigned char cmd)
   {
      m_statusByte = cmd;
      switch (m_statusByte)
      {
      case thermostat3_sOff: m_onOff->set(false);
         break;
      case thermostat3_sOn: m_onOff->set(true);
         break;

      case thermostat3_sUp: m_upDown->set(yApi::historization::EUpDownStopCommand::kUp);
         break;
      case thermostat3_sDown: m_upDown->set(yApi::historization::EUpDownStopCommand::kDown);
         break;
      case thermostat3_sRunUp: m_runUpDown->set(yApi::historization::EUpDownStopCommand::kUp);
         break;
      case thermostat3_sRunDown: m_runUpDown->set(yApi::historization::EUpDownStopCommand::kDown);
         break;
      case thermostat3_sStop: m_upDown->set(yApi::historization::EUpDownStopCommand::kStop);
         m_runUpDown->set(yApi::historization::EUpDownStopCommand::kStop);
         break;

      default:
         throw shared::exception::CInvalidParameter("state, " + boost::lexical_cast<std::string>(m_statusByte));
      }
   }

   void CThermostat3MertikG6RHxT1::toProtocolState(unsigned char& cmd) const
   {
      cmd = m_statusByte;
   }
} // namespace rfxcomMessages


