#include "stdafx.h"
#include "DeviceIdentifier.h"

namespace xplrules {


   CDeviceIdentifier::CDeviceIdentifier(const std::string & id, const std::string & commercialName, const xplcore::CXplMessageSchemaIdentifier & readingProtocol, const xplcore::CXplMessageSchemaIdentifier & writingProtocol)
      :m_id(id), m_commercialName(commercialName), m_xplProtocolReading(readingProtocol), m_xplProtocolWriting(writingProtocol)
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

   const xplcore::CXplMessageSchemaIdentifier & CDeviceIdentifier::getReadingXplProtocol()
   {
      return m_xplProtocolReading;
   }

 
   const xplcore::CXplMessageSchemaIdentifier & CDeviceIdentifier::getWritingXplProtocol()
   {
      return m_xplProtocolWriting;

   }

} //namespace xplrules


