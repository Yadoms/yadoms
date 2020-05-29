#include "stdafx.h"
#include "TemperatureIo.h"
#include "SingleTemperature.h"
#include "Identification.h"
#include <shared/Log.h>

namespace device
{
const boost::posix_time::time_duration CTemperatureIo::HistorizationPeriod(boost::posix_time::seconds(5));

CTemperatureIo::CTemperatureIo(EOneWireFamily family,
                               const std::string &id,
                               boost::shared_ptr<ioInterfaces::ITemperatureIo> io)
    : m_identification(boost::make_shared<device::CIdentification>(family, id, "DS28EA00")),
      m_io(io),
      m_ioA(boost::make_shared<yApi::historization::CSwitch>("io_A")),
      m_ioB(boost::make_shared<yApi::historization::CSwitch>("io_B")),
      m_temperature(boost::make_shared<yApi::historization::CTemperature>("temperature")),
      m_keywordsWithTemperature({m_ioA, m_ioB, m_temperature}),
      m_keywordsWithoutTemperature({m_ioA, m_ioB}),
      m_keywords(&m_keywordsWithTemperature)
{
   BOOST_ASSERT_MSG(m_identification->family() == kTemperatureIo, "Invalid family number");
}

void CTemperatureIo::setConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                      const boost::shared_ptr<shared::CDataContainer> &configuration)
{
   YADOMS_LOG(error) << "Try to apply a device configuration to an unconfigurable device";
}

void CTemperatureIo::read() const
{
   m_ioA->set(m_io->readIo(0));
   m_ioB->set(m_io->readIo(1));

   double temperature = m_io->readTemperature();
   if (CSingleTemperature::isTemperatureValid(temperature))
   {
      m_temperature->set(temperature);
      m_keywords = const_cast<std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> *>(&m_keywordsWithTemperature);
   }
   else
   {
      m_keywords = const_cast<std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> *>(&m_keywordsWithoutTemperature);
   }
}

void CTemperatureIo::write(const std::string &keyword, const std::string &command)
{
   boost::shared_ptr<yApi::historization::CSwitch> kw;
   unsigned char unit;

   if (m_ioA->getKeyword() == keyword)
   {
      kw = m_ioA;
      unit = 0;
   }
   else if (m_ioB->getKeyword() == keyword)
   {
      kw = m_ioB;
      unit = 1;
   }
   else
   {
      YADOMS_LOG(error) << "Unknown keyword " << keyword;
      return;
   }

   if (kw->getAccessMode() != yApi::EKeywordAccessMode::kGetSetValue)
   {
      YADOMS_LOG(error) << "Try to drive the read-only keyword " << keyword;
      return;
   }
   kw->setCommand(command);
   m_io->writeIo(unit, kw->get());
}

bool CTemperatureIo::hasRelevantValue()
{
   if (m_keywords->empty())
      return false;

   const auto now = shared::currentTime::Provider().now();

   if (now < (m_lastHistorizationDate + HistorizationPeriod))
      return false;

   m_lastHistorizationDate = now;
   return true;
}
} // namespace device
