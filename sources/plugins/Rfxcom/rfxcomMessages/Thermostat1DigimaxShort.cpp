#include "stdafx.h"
#include "Thermostat1DigimaxShort.h"
#include "RFXtrxHelpers.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CThermostat1DigimaxShort::CThermostat1DigimaxShort()
      : m_currentTemperature(boost::make_shared<yApi::historization::CTemperature>("temperature")),
        m_keywords({m_currentTemperature})
   {
   }

   CThermostat1DigimaxShort::~CThermostat1DigimaxShort()
   {
   }

   std::string CThermostat1DigimaxShort::getModel() const
   {
      return "Digimax short (no set point)";
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& CThermostat1DigimaxShort::keywords() const
   {
      return m_keywords;
   }

   void CThermostat1DigimaxShort::set(const std::string& /*yadomsCommand*/)
   {
   }

   void CThermostat1DigimaxShort::reset()
   {
      m_currentTemperature->set(0.0);
   }

   void CThermostat1DigimaxShort::setFromProtocolState(const RBUF& thermostat1Rbuf)
   {
      m_currentTemperature->set(static_cast<double>(thermostat1Rbuf.THERMOSTAT1.temperature));
   }

   void CThermostat1DigimaxShort::toProtocolState(RBUF& thermostat1Rbuf) const
   {
      thermostat1Rbuf.THERMOSTAT1.temperature = reinterpret_cast<unsigned char>(m_currentTemperature.get());
      thermostat1Rbuf.THERMOSTAT1.set_point = 0;
      thermostat1Rbuf.THERMOSTAT1.mode = 0x00; // heating
      thermostat1Rbuf.THERMOSTAT1.status = thermostat1_sNoDemand;
   }
} // namespace rfxcomMessages


