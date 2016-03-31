#include "stdafx.h"
#include "Identification.h"
#include <shared/DataContainer.h>


namespace device {

std::string buildDeviceName(EOneWireFamily family, const std::string& id)
{
   std::stringstream ss;
   ss << std::hex << family;
   ss << '-';
   ss << id;
   return std::string(ss.str());
}

CIdentification::CIdentification(EOneWireFamily family, const std::string& id, const std::string& model)
   :m_family(family), m_id(id), m_deviceName(buildDeviceName(family, id)), m_model(model)
{
}

CIdentification::~CIdentification()
{
}

void CIdentification::declare(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;

      std::stringstream ss;
      ss << std::hex << m_family;
      details.set("Family", ss.str());

      details.set("Serial", m_id);

      context->declareDevice(m_deviceName, m_model, details);
   }
}

EOneWireFamily CIdentification::family() const
{
   return m_family;
}

std::string CIdentification::id() const
{
   return m_id;
}

const std::string& CIdentification::deviceName() const
{
   return m_deviceName;
}

const std::string& CIdentification::model() const
{
   return m_model;
}

} // namespace device