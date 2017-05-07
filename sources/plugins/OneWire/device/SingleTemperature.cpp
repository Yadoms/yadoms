#include "stdafx.h"
#include "SingleTemperature.h"
#include "Identification.h"
#include <shared/Log.h>

namespace device
{
   CSingleTemperature::CSingleTemperature(EOneWireFamily family,
                                          const std::string& id,
                                          const std::string& model,
                                          boost::shared_ptr<ioInterfaces::ITemperature> io,
                                          EOneWireFamily expectedFamily)
      :m_identification(boost::make_shared<device::CIdentification>(family, id, model)),
      m_io(io),
      m_temperature(boost::make_shared<yApi::historization::CTemperature>("temperature")),
      m_allKeywords({ m_temperature }),
      m_noKeywords(),
      m_keywords(&m_allKeywords)
   {
      BOOST_ASSERT_MSG(m_identification->family() == expectedFamily, "Invalid family number");
   }

   CSingleTemperature::~CSingleTemperature()
   {
   }

   void CSingleTemperature::setConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                             const shared::CDataContainer& configuration)
   {
      YADOMS_LOG(error) << "Try to apply a device configuration to an unconfigurable device";
   }

   void CSingleTemperature::read() const
   {
      double temperature = m_io->read();
      if (isTemperatureValid(temperature))
      {
         m_temperature->set(temperature);
         m_keywords = const_cast<std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >*>(&m_allKeywords);
      }
      else
      {
         m_keywords = const_cast<std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >*>(&m_noKeywords);
      }
   }

   void CSingleTemperature::write(const std::string& keyword, const std::string& command)
   {
      YADOMS_LOG(error) << "Try to drive the read-only keyword " << keyword;
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
