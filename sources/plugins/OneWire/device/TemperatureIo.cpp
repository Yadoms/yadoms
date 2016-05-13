#include "stdafx.h"
#include "TemperatureIo.h"
#include "SingleTemperature.h"
#include "Identification.h"

namespace device {

CTemperatureIo::CTemperatureIo(EOneWireFamily family, const std::string& id, boost::shared_ptr<yApi::IYPluginApi> context, boost::shared_ptr<ioInterfaces::ITemperatureIo> io)
   :m_identification(boost::make_shared<device::CIdentification>(family, id, "DS28EA00")), m_context(context), m_io(io),
   m_ioA(boost::make_shared<yApi::historization::CSwitch>("io_A")),
   m_ioB(boost::make_shared<yApi::historization::CSwitch>("io_B")),
   m_temperature(boost::make_shared<yApi::historization::CTemperature>("temperature"))
{
   BOOST_ASSERT_MSG(m_identification->family() == kTemperatureIo, "Invalid family number");
}

CTemperatureIo::~CTemperatureIo()
{
}

boost::shared_ptr<const IIdentification> CTemperatureIo::ident() const
{
   return m_identification;
}

void CTemperatureIo::declare()
{
   m_identification->declare(m_context);

   if (!m_context->keywordExists(m_identification->deviceName(), *m_ioA))
      m_context->declareKeyword(m_identification->deviceName(), *m_ioA);
   if (!m_context->keywordExists(m_identification->deviceName(), *m_ioB))
      m_context->declareKeyword(m_identification->deviceName(), *m_ioB);
   if (!m_context->keywordExists(m_identification->deviceName(), *m_temperature))
      m_context->declareKeyword(m_identification->deviceName(), *m_temperature);
}

void CTemperatureIo::historize()
{
   std::vector<boost::shared_ptr<yApi::historization::IHistorizable> > keywords;

   m_ioA->set(m_io->readIo(0));
   keywords.push_back(m_ioA);

   m_ioB->set(m_io->readIo(1));
   keywords.push_back(m_ioB);

   double temperature = m_io->readTemperature();
   if (CSingleTemperature::isTemperatureValid(temperature))
   {
      m_temperature->set(temperature);
      keywords.push_back(m_temperature);
   }

   m_context->historizeData(ident()->deviceName(), keywords);
}

void CTemperatureIo::set(const std::string& keyword, const std::string& command)
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
      std::cerr << "Unknown keyword " << keyword << std::endl;
      return;
   }

   if (kw->getAccessMode() != yApi::EKeywordAccessMode::kGetSetValue)
   {
      std::cerr << "Try to drive the read-only keyword " << keyword << std::endl;
      return;
   }
   kw->setCommand(command);
   m_io->writeIo(unit, kw->get());
}

} // namespace device
