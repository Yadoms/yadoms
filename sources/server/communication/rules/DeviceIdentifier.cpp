#include "stdafx.h"
#include "DeviceIdentifier.h"

namespace communication { namespace rules {


   CDeviceIdentifier::CDeviceIdentifier(const std::string & id)
      :m_id(id)
   {
   }

   CDeviceIdentifier::CDeviceIdentifier(const std::string & id, const std::string & commercialName)
      :m_id(id), m_commercialName(commercialName)
   {
   }

   CDeviceIdentifier::~CDeviceIdentifier()
   {
   }

   const std::string & CDeviceIdentifier::getId()
   {
      return m_id;
   }


   const std::string & CDeviceIdentifier::getCommercialName()
   {
      if(m_commercialName.empty())
         return m_id;
      return m_commercialName;
   }

} //namespace rules
} //namespace communication

