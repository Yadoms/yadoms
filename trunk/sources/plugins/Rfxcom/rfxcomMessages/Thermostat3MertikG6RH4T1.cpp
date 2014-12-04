#include "stdafx.h"
#include "Thermostat3MertikG6RH4T1.h"
#include "RFXtrxHelpers.h"

#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CThermostat3MertikG6RH4T1::CThermostat3MertikG6RH4T1()
   :m_statusByte(0), m_onOff("onOff"), m_UpDown("upDown", yApi::EKeywordAccessMode::kGetSet), m_RunUpDown("runUpDown", yApi::EKeywordAccessMode::kGetSet)
{
}

std::string CThermostat3MertikG6RH4T1::getModel() const
{
   return "Mertik G6R-H4T1";
}

void CThermostat3MertikG6RH4T1::declare(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& deviceName) const
{
   context->declareKeyword(deviceName, m_onOff);
   context->declareKeyword(deviceName, m_UpDown);
   context->declareKeyword(deviceName, m_RunUpDown);
}

void CThermostat3MertikG6RH4T1::historize(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& deviceName) const
{
   context->historizeData(deviceName, m_onOff);
   context->historizeData(deviceName, m_UpDown);
   context->historizeData(deviceName, m_RunUpDown);
}

void CThermostat3MertikG6RH4T1::set(const std::string& keyword, const std::string& yadomsCommand)
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
   else if (boost::iequals(keyword, m_RunUpDown.getKeyword()))
   {
      m_RunUpDown.set(yadomsCommand);
      switch(m_RunUpDown.get())
      {
      case yApi::historization::EUpDownStopCommand::kUpValue: m_statusByte = thermostat3_sRunUp; break;
      case yApi::historization::EUpDownStopCommand::kDownValue: m_statusByte = thermostat3_sRunDown; break;
      case yApi::historization::EUpDownStopCommand::kStopValue: m_statusByte = thermostat3_sStop; break;
      default: throw shared::exception::CInvalidParameter("yadomsCommand");
      }
   }
}

void CThermostat3MertikG6RH4T1::reset()
{
   m_onOff.set(false);
   m_UpDown.set(yApi::historization::EUpDownStopCommand::kStopValue);
   m_RunUpDown.set(yApi::historization::EUpDownStopCommand::kStopValue);
}

void CThermostat3MertikG6RH4T1::setFromProtocolState(unsigned char cmd)
{
   m_statusByte = cmd;
   switch(m_statusByte)
   {
   case thermostat3_sOff      : m_onOff.set(false); break;
   case thermostat3_sOn       : m_onOff.set(true); break;

   case thermostat3_sUp       : m_UpDown.set(yApi::historization::EUpDownStopCommand::kUpValue); break;
   case thermostat3_sDown     : m_UpDown.set(yApi::historization::EUpDownStopCommand::kDownValue); break;
   case thermostat3_sRunUp    : m_RunUpDown.set(yApi::historization::EUpDownStopCommand::kUpValue); break;
   case thermostat3_sRunDown  : m_RunUpDown.set(yApi::historization::EUpDownStopCommand::kDownValue); break;
   case thermostat3_sStop     : m_UpDown.set(yApi::historization::EUpDownStopCommand::kStopValue); m_RunUpDown.set(yApi::historization::EUpDownStopCommand::kStopValue); break;

   default:
      {
         BOOST_ASSERT_MSG(false, "Invalid state");
         throw shared::exception::CInvalidParameter("state");
      }
   }
}

void CThermostat3MertikG6RH4T1::toProtocolState(unsigned char& cmd) const
{
   cmd = m_statusByte;
}

} // namespace rfxcomMessages