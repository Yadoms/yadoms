#include "stdafx.h"
#include "HighPrecisionLiBatteryMonitor.h"
#include "Identification.h"
#include <shared/Log.h>

namespace device {

CHighPrecisionLiBatteryMonitor::CHighPrecisionLiBatteryMonitor(EOneWireFamily family, const std::string& id, boost::shared_ptr<yApi::IYPluginApi> context, boost::shared_ptr<ioInterfaces::IHighPrecisionLiBatteryMonitor> io)
   :m_identification(boost::make_shared<device::CIdentification>(family, id, "DS2760")), m_context(context), m_io(io),
   m_kwIo(boost::make_shared<yApi::historization::CSwitch>("io")),
   m_kwTemperature(boost::make_shared<yApi::historization::CTemperature>("temperature")),
   m_kwVis(boost::make_shared<yApi::historization::CVoltage>("vis")),
   m_kwVolt(boost::make_shared<yApi::historization::CVoltage>("volt"))
{
   BOOST_ASSERT_MSG(m_identification->family() == kHighPrecisionLiBatteryMonitor, "Invalid family number");

   m_keywords.push_back(m_kwIo);
   m_keywords.push_back(m_kwTemperature);
   m_keywords.push_back(m_kwVis);
   m_keywords.push_back(m_kwVolt);
}

CHighPrecisionLiBatteryMonitor::~CHighPrecisionLiBatteryMonitor()
{
}

boost::shared_ptr<const IIdentification> CHighPrecisionLiBatteryMonitor::ident() const
{
   return m_identification;
}

void CHighPrecisionLiBatteryMonitor::declare()
{
   m_identification->declare(m_context);

   for (std::vector<boost::shared_ptr<yApi::historization::IHistorizable> >::const_iterator keyword = m_keywords.begin(); keyword != m_keywords.end(); ++keyword)
      if (!m_context->keywordExists(m_identification->deviceName(), **keyword))
         m_context->declareKeyword(m_identification->deviceName(), **keyword);
}

void CHighPrecisionLiBatteryMonitor::historize()
{
   m_kwIo->set(m_io->readIo());
   m_kwTemperature->set(m_io->readTemperature());
   m_kwVis->set(m_io->readVis());
   m_kwVolt->set(m_io->readVolt());

   m_context->historizeData(ident()->deviceName(), m_keywords);
}

void CHighPrecisionLiBatteryMonitor::set(const std::string& keyword, const std::string& command)
{
   if (m_kwIo->getKeyword() != keyword)
   {
      YADOMS_LOG(error) << "Unknown keyword " << keyword;
      return;
   }

   m_kwIo->setCommand(command);
   m_io->writeIo(m_kwIo->get());
}

} // namespace device
