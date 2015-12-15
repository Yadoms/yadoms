#include "stdafx.h"
#include "Thermostat3MertikG6RH4S.h"
#include "RFXtrxHelpers.h"

#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CThermostat3MertikG6RH4S::CThermostat3MertikG6RH4S()
   :m_onOff("onOff"), m_UpDown("upDown", yApi::EKeywordAccessMode::kGetSet), m_statusByte(0)
{
}

std::string CThermostat3MertikG6RH4S::getModel() const
{
   return "Mertik G6R-H4S";
}

void CThermostat3MertikG6RH4S::declare(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& deviceName) const
{
   context->declareKeyword(deviceName, m_onOff);
   context->declareKeyword(deviceName, m_UpDown);
}

void CThermostat3MertikG6RH4S::historize(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& deviceName) const
{
   context->historizeData(deviceName, m_onOff);
   context->historizeData(deviceName, m_UpDown);
}

void CThermostat3MertikG6RH4S::set(const std::string& keyword, const std::string& yadomsCommand)
{
   if (boost::iequals(keyword, m_onOff.getKeyword()))
   {
      m_onOff.set(yadomsCommand); m_statusByte = m_onOff.get() ? thermostat3_sOn : thermostat3_sOff;
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

void CThermostat3MertikG6RH4S::reset()
{
   m_onOff.set(false);
   m_UpDown.set(yApi::historization::EUpDownStopCommand::kStopValue);
}

void CThermostat3MertikG6RH4S::setFromProtocolState(unsigned char cmd)
{
   m_statusByte = cmd;
   switch(m_statusByte)
   {
   case thermostat3_sOff      : m_onOff.set(false); break;
   case thermostat3_sOn       : m_onOff.set(true); break;

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

void CThermostat3MertikG6RH4S::toProtocolState(unsigned char& cmd) const
{
   cmd = m_statusByte;
}

} // namespace rfxcomMessages
