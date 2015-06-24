#include "stdafx.h"
#include "DigitalPotentiometer.h"
#include "Identification.h"
#include <shared/Log.h>

namespace device {

CDigitalPotentiometer::CDigitalPotentiometer(EOneWireFamily family, const std::string& id, boost::shared_ptr<yApi::IYPluginApi> context, boost::shared_ptr<ioInterfaces::IDigitalPotentiometer> io)
   :m_identification(boost::make_shared<device::CIdentification>(family, id, "DS2890")), m_context(context), m_io(io),
   m_potentiometerMode(boost::make_shared<yApi::historization::CSwitch>("PotentiometerMode")),
   m_dim(boost::make_shared<yApi::historization::CDimmable>("dim"))
{
   BOOST_ASSERT_MSG(m_identification->family() == kDigitalPotentiometer, "Invalid family number");
}

CDigitalPotentiometer::~CDigitalPotentiometer()
{
}

boost::shared_ptr<const IIdentification> CDigitalPotentiometer::ident() const
{
   return m_identification;
}

void CDigitalPotentiometer::declare()
{
   m_identification->declare(m_context);

   if (!m_context->keywordExists(m_identification->deviceName(), *m_potentiometerMode))
      m_context->declareKeyword(m_identification->deviceName(), *m_potentiometerMode);
   if (!m_context->keywordExists(m_identification->deviceName(), *m_dim))
      m_context->declareKeyword(m_identification->deviceName(), *m_dim);
}

void CDigitalPotentiometer::historize()
{
   std::vector<boost::shared_ptr<yApi::historization::IHistorizable> > keywords;

   m_potentiometerMode->set(m_io->readPotentiometerMode());
   keywords.push_back(m_potentiometerMode);

   m_dim->set(m_io->readDim());
   keywords.push_back(m_dim);

   m_context->historizeData(ident()->deviceName(), keywords);
}

void CDigitalPotentiometer::set(const std::string& keyword, const std::string& command)
{
   boost::shared_ptr<yApi::historization::CSwitch> kw;

   if (m_potentiometerMode->getKeyword() == keyword)
   {
      m_potentiometerMode->set(command);
      m_io->writePotentiometerMode(m_potentiometerMode->get());
   }
   else if (m_dim->getKeyword() == keyword)
   {
      m_dim->set(command);
      m_io->writeDim(m_dim->switchLevel());
   }
   else
   {
      YADOMS_LOG(error) << "Unknown keyword " << keyword;
      return;
   }
}

} // namespace device