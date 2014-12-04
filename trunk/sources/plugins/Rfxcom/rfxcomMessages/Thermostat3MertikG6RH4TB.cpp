#include "stdafx.h"
#include "Thermostat3MertikG6RH4TB.h"
#include "RFXtrxHelpers.h"

#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CThermostat3MertikG6RH4TB::CThermostat3MertikG6RH4TB()
   :m_statusByte(0), m_onOff("onOff"), m_onOff2("onOff2"), m_UpDown("upDown", yApi::EKeywordAccessMode::kGetSet)
{
}

std::string CThermostat3MertikG6RH4TB::getModel() const
{
   return "Mertik G6R-H4TB / G6-H4T / G6R-H4T21-Z22";
}

void CThermostat3MertikG6RH4TB::declare(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& deviceName) const
{
   context->declareKeyword(deviceName, m_onOff);
   context->declareKeyword(deviceName, m_onOff2);
   context->declareKeyword(deviceName, m_UpDown);
}

void CThermostat3MertikG6RH4TB::historize(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& deviceName) const
{
   context->historizeData(deviceName, m_onOff);
   context->historizeData(deviceName, m_onOff2);
   context->historizeData(deviceName, m_UpDown);
}

void CThermostat3MertikG6RH4TB::set(const std::string& keyword, const std::string& yadomsCommand)
{
   if (boost::iequals(keyword, m_onOff.getKeyword()))
   {
      m_onOff.set(yadomsCommand); m_statusByte = m_onOff.get() ? thermostat3_sOn : thermostat3_sOff;
   }
   else if (boost::iequals(keyword, m_onOff2.getKeyword()))
   {
      m_onOff2.set(yadomsCommand); m_statusByte = m_onOff2.get() ? thermostat3_On2nd : thermostat3_Off2nd;
   }
   else if (boost::iequals(keyword, m_UpDown.getKeyword()))
   {
      m_UpDown.set(yadomsCommand);
      switch(m_UpDown.get())
      {
      case yApi::historization::EUpDownStopCommand::kUpValue: m_statusByte = thermostat3_sUp; break;
      case yApi::historization::EUpDownStopCommand::kDownValue: m_statusByte = thermostat3_sDown; break;
      case yApi::historization::EUpDownStopCommand::kStopValue: m_statusByte = thermostat3_sStop; break;
      default: throw shared::exception::CInvalidParameter("yadomsCommand");
      }
   }
}

void CThermostat3MertikG6RH4TB::reset()
{
   m_onOff.set(false);
   m_onOff2.set(false);
   m_UpDown.set(yApi::historization::EUpDownStopCommand::kStopValue);
}

void CThermostat3MertikG6RH4TB::setFromProtocolState(unsigned char cmd)
{
   m_statusByte = cmd;
   switch(m_statusByte)
   {
   case thermostat3_sOff      : m_onOff.set(false); break;
   case thermostat3_sOn       : m_onOff.set(true); break;

   case thermostat3_Off2nd    : m_onOff2.set(false); break;
   case thermostat3_On2nd     : m_onOff2.set(true); break;

   case thermostat3_sUp       : m_UpDown.set(yApi::historization::EUpDownStopCommand::kUpValue); break;
   case thermostat3_sDown     : m_UpDown.set(yApi::historization::EUpDownStopCommand::kDownValue); break;
   case thermostat3_sStop     : m_UpDown.set(yApi::historization::EUpDownStopCommand::kStopValue); break;

   default:
      {
         BOOST_ASSERT_MSG(false, "Invalid state");
         throw shared::exception::CInvalidParameter("state");
      }
   }
}

void CThermostat3MertikG6RH4TB::toProtocolState(unsigned char& cmd) const
{
   cmd = m_statusByte;
}

} // namespace rfxcomMessages