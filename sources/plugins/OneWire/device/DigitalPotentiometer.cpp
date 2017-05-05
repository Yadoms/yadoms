#include "stdafx.h"
#include "DigitalPotentiometer.h"
#include "Identification.h"
#include <shared/Log.h>

namespace device
{
   CDigitalPotentiometer::CDigitalPotentiometer(EOneWireFamily family,
                                                const std::string& id,
                                                boost::shared_ptr<ioInterfaces::IDigitalPotentiometer> io)
      :m_identification(boost::make_shared<device::CIdentification>(family, id, "DS2890")),
      m_io(io),
      m_potentiometerMode(boost::make_shared<yApi::historization::CSwitch>("PotentiometerMode")),
      m_dim(boost::make_shared<yApi::historization::CDimmable>("dim")),
      m_keywords({ m_potentiometerMode, m_dim })
   {
      BOOST_ASSERT_MSG(m_identification->family() == kDigitalPotentiometer, "Invalid family number");
   }

   CDigitalPotentiometer::~CDigitalPotentiometer()
   {
   }

   void CDigitalPotentiometer::setConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                                const shared::CDataContainer& configuration)
   {
      YADOMS_LOG(error) << "Try to apply a device configuration to an unconfigurable device";
   }

   void CDigitalPotentiometer::read() const
   {
      m_potentiometerMode->set(m_io->readPotentiometerMode());
      m_dim->set(m_io->readDim());
   }

   void CDigitalPotentiometer::write(const std::string& keyword, const std::string& command)
   {
      boost::shared_ptr<yApi::historization::CSwitch> kw;

      if (m_potentiometerMode->getKeyword() == keyword)
      {
         m_potentiometerMode->setCommand(command);
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
