#include "stdafx.h"
#include "Thermostat1Digimax.h"
#include "RFXtrxHelpers.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CThermostat1Digimax::CThermostat1Digimax()
      : m_currentTemperature(boost::make_shared<yApi::historization::CTemperature>("temperature")),
        m_setPoint(boost::make_shared<yApi::historization::CTemperature>("setPoint", yApi::EKeywordAccessMode::kGetSet)),
        m_keywords({m_currentTemperature})
   {
   }

   CThermostat1Digimax::~CThermostat1Digimax()
   {
   }

   std::string CThermostat1Digimax::getModel() const
   {
      return "Digimax TLX7506";
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& CThermostat1Digimax::keywords() const
   {
      return m_keywords;
   }

   void CThermostat1Digimax::set(const std::string& yadomsCommand)
   {
      m_currentTemperature->set(0.0);
      m_setPoint->setCommand(yadomsCommand);
   }

   void CThermostat1Digimax::reset()
   {
      m_currentTemperature->set(0.0);
      m_setPoint->set(0.0);
   }

   void CThermostat1Digimax::setFromProtocolState(const RBUF& thermostat1Rbuf)
   {
      m_currentTemperature->set(static_cast<double>(thermostat1Rbuf.THERMOSTAT1.temperature));
      m_setPoint->set(static_cast<double>(thermostat1Rbuf.THERMOSTAT1.set_point));
   }

   void CThermostat1Digimax::toProtocolState(RBUF& thermostat1Rbuf) const
   {
      thermostat1Rbuf.THERMOSTAT1.temperature = static_cast<unsigned char>(m_currentTemperature->get());
      thermostat1Rbuf.THERMOSTAT1.set_point = static_cast<unsigned char>(m_setPoint->get());
      thermostat1Rbuf.THERMOSTAT1.mode = 0x00; // heating
      thermostat1Rbuf.THERMOSTAT1.status = thermostat1_sDemand;
   }
} // namespace rfxcomMessages


