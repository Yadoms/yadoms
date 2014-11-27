#include "stdafx.h"
#include "Thermostat1DigimaxShort.h"
#include "RFXtrxHelpers.h"

#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CThermostat1DigimaxShort::CThermostat1DigimaxShort()
   :m_currentTemperature("temperature")
{
}

CThermostat1DigimaxShort::~CThermostat1DigimaxShort()
{
}
   
std::string CThermostat1DigimaxShort::getModel() const
{
   return "Digimax short (no set point)";
}

void CThermostat1DigimaxShort::declare(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const
{
   context->declareKeyword(deviceName, m_currentTemperature);
}

void CThermostat1DigimaxShort::historize(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const
{
   context->historizeData(deviceName, m_currentTemperature);
}

void CThermostat1DigimaxShort::set(const std::string& /*yadomsCommand*/)
{
}

void CThermostat1DigimaxShort::reset()
{
   m_currentTemperature.set(0.0);
}

void CThermostat1DigimaxShort::setFromProtocolState(const RBUF& thermostat1Rbuf)
{
   m_currentTemperature.set((double)thermostat1Rbuf.THERMOSTAT1.temperature);
}

void CThermostat1DigimaxShort::toProtocolState(RBUF& thermostat1Rbuf) const
{
   thermostat1Rbuf.THERMOSTAT1.temperature  = (unsigned char)m_currentTemperature.get();
   thermostat1Rbuf.THERMOSTAT1.set_point  = 0;
   thermostat1Rbuf.THERMOSTAT1.mode = 0x00; // heating
   thermostat1Rbuf.THERMOSTAT1.status = thermostat1_sNoDemand;
}

} // namespace rfxcomMessages