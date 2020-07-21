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
      YADOMS_LOG(information) << "Pairing mode : " << m_configuration.get<std::string>("PairingMode.activeSectionText");
      YADOMS_LOG(information) << "Bridge IP is :  "
         << (m_configuration.get<std::string>("PairingMode.content.Manual.content.IPAddress").empty()
                ? "Automatic Mode"
                : m_configuration.get<std::string>("PairingMode.content.Manual.content.IPAddress"));
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

std::string CConfiguration::getIPAddress() const
{
   if (getPairingMode() != kAuto)
      return m_configuration.get<std::string>("PairingMode.content.Manual.content.IPAddress");
   return m_ipAddress;
}

void CConfiguration::setIPAddress(const std::string& ipAddress)
{
   m_ipAddress = ipAddress;
}

EPairingMode CConfiguration::getPairingMode() const
{
   return m_configuration.get<std::string>("PairingMode.activeSectionText") == "Auto" ? kAuto : kManual;
}
