#include "stdafx.h"
#include "Thermostat1Digimax.h"
#include "RFXtrxHelpers.h"

#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CThermostat1Digimax::CThermostat1Digimax()
   :m_currentTemperature("temperature"), m_setPoint("setPoint", yApi::EKeywordAccessMode::kGetSet)
{
}

CThermostat1Digimax::~CThermostat1Digimax()
{
}
   
std::string CThermostat1Digimax::getModel() const
{
   return "Digimax TLX7506";
}

void CThermostat1Digimax::declare(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& deviceName) const
{
   context->declareKeyword(deviceName, m_currentTemperature);
   context->declareKeyword(deviceName, m_setPoint);
}

void CThermostat1Digimax::historize(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& deviceName) const
{
   context->historizeData(deviceName, m_currentTemperature);
   context->historizeData(deviceName, m_setPoint);
}

void CThermostat1Digimax::set(const std::string& yadomsCommand)
{
   m_currentTemperature.set(0.0);
   m_setPoint.setCommand(yadomsCommand);
}

void CThermostat1Digimax::reset()
{
   m_currentTemperature.set(0.0);
   m_setPoint.set(0.0);
}

void CThermostat1Digimax::setFromProtocolState(const RBUF& thermostat1Rbuf)
{
   m_currentTemperature.set((double)thermostat1Rbuf.THERMOSTAT1.temperature);
   m_setPoint.set((double)thermostat1Rbuf.THERMOSTAT1.set_point);
}

void CThermostat1Digimax::toProtocolState(RBUF& thermostat1Rbuf) const
{
   thermostat1Rbuf.THERMOSTAT1.temperature  = (unsigned char)m_currentTemperature.get();
   thermostat1Rbuf.THERMOSTAT1.set_point  = (unsigned char)m_setPoint.get();
   thermostat1Rbuf.THERMOSTAT1.mode = 0x00; // heating
   thermostat1Rbuf.THERMOSTAT1.status = thermostat1_sDemand;
}

} // namespace rfxcomMessages