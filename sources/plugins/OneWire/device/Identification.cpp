#include "stdafx.h"
#include "Identification.h"
#include <shared/DataContainer.h>


namespace device
{
   std::string buildDeviceName(EOneWireFamily family,
                               const std::string& id)
   {
      std::stringstream ss;
      ss << std::hex << family;
      ss << '-';
      ss << id;
      return std::string(ss.str());
   }

   shared::CDataContainer buildDeviceDetails(EOneWireFamily family,
                                             const std::string& id)
   {
      shared::CDataContainer details;

      std::stringstream ss;
      ss << std::hex << family;
      details.set("Family", ss.str());

      details.set("Serial", id);

      return details;
   }

   CIdentification::CIdentification(EOneWireFamily family,
                                    const std::string& id,
                                    const std::string& model)
      :m_family(family),
      m_id(id),
      m_deviceName(buildDeviceName(family, id)),
      m_model(model),
      m_details(buildDeviceDetails(family, id))
   {
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

   const shared::CDataContainer& CIdentification::details() const
   {
      return m_details;
   }

} // namespace device