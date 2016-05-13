#include "stdafx.h"
#include "SingleTemperature.h"
#include "Identification.h"

namespace device {
   
CSingleTemperature::CSingleTemperature(EOneWireFamily family, const std::string& id, const std::string& model, boost::shared_ptr<yApi::IYPluginApi> context, boost::shared_ptr<ioInterfaces::ITemperature> io, EOneWireFamily expectedFamily)
   :m_identification(boost::make_shared<device::CIdentification>(family, id, model)), m_context(context), m_io(io), m_temperature("temperature")
{
   BOOST_ASSERT_MSG(m_identification->family() == expectedFamily, "Invalid family number");
}

CSingleTemperature::~CSingleTemperature()
{
}

boost::shared_ptr<const IIdentification> CSingleTemperature::ident() const
{
   return m_identification;
}

void CSingleTemperature::declare()
{
   m_identification->declare(m_context);

   if (!m_context->keywordExists(m_identification->deviceName(), m_temperature))
      m_context->declareKeyword(m_identification->deviceName(), m_temperature);
}

void CSingleTemperature::historize()
{
   double temperature = m_io->read();
   if (isTemperatureValid(temperature))
   {
      m_temperature.set(temperature);
      m_context->historizeData(ident()->deviceName(), m_temperature);
   }
}

void CSingleTemperature::set(const std::string& keyword, const std::string& command)
{
   std::cerr << "Try to drive the read-only keyword " << keyword << std::endl;
}

bool CSingleTemperature::isTemperatureValid(double temperature)
{
   if (temperature <= -300 || temperature >= 381)
      return false;

   // Some devices has a power-on value at 85°, we have to filter it
   if (temperature == 85)
      return false;

   return true;
}

} // namespace device