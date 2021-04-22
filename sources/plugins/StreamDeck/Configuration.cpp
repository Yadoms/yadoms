#include "stdafx.h"
#include "Configuration.h"
#include <shared/Log.h>

CConfiguration::~CConfiguration()
{
}

void CConfiguration::initializeWith(const boost::shared_ptr<shared::CDataContainer>& data)
{
   m_configuration.initializeWith(data);
}

void CConfiguration::trace() const
{
   try
   {
      // Get simple parameters
      YADOMS_LOG(information) << "StreamDeck configuration, parameter 'StringParameter' is "
         << (m_configuration.get<std::string>("StringParameter").empty()
                ? "empty"
                : m_configuration.get<std::string>("StringParameter"));
      YADOMS_LOG(information) << "StreamDeck configuration, parameter 'BoolParameter' is " << m_configuration.get<bool>(
            "BoolParameter");
      YADOMS_LOG(information) << "StreamDeck configuration, parameter 'DecimalParameter' is " << m_configuration.get<
            double>("DecimalParameter");
      YADOMS_LOG(information) << "StreamDeck configuration, parameter 'IntParameter' is " << m_configuration.get<int
         >("IntParameter");
   }
   catch (const shared::exception::CInvalidParameter& e)
   {
      YADOMS_LOG(error) << "Parameter not found : " << e.what();
   }
   catch (const shared::exception::COutOfRange& e)
   {
      YADOMS_LOG(error) << "Parameter value out of range : " << e.what();
   }
}

std::string CConfiguration::getUsbDevice() const
{
   return m_configuration.get<std::string>("UsbDevices");
}

