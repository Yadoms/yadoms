#include "stdafx.h"
#include "DeviceIdentifier.h"

namespace xplrules
{
   CDeviceIdentifier::CDeviceIdentifier(const std::string& id,
                                        const std::string& commercialName,
                                        const xplcore::CXplMessageSchemaIdentifier& readingProtocol,
                                        const xplcore::CXplMessageSchemaIdentifier& writingProtocol,
                                        const boost::shared_ptr<shared::CDataContainer>& innerDetails)
      : m_id(id),
        m_commercialName(commercialName),
        m_xplProtocolReading(readingProtocol),
        m_xplProtocolWriting(writingProtocol),
        m_innerDetails(innerDetails)
   {
   }

   CDeviceIdentifier::~CDeviceIdentifier()
   {
   }

   const std::string& CDeviceIdentifier::getId() const
   {
      return m_id;
   }


   const std::string& CDeviceIdentifier::getCommercialName() const
   {
      if (m_commercialName.empty())
         return m_id;
      return m_commercialName;
   }

   const xplcore::CXplMessageSchemaIdentifier& CDeviceIdentifier::getReadingXplProtocol() const
   {
      return m_xplProtocolReading;
   }


   const xplcore::CXplMessageSchemaIdentifier& CDeviceIdentifier::getWritingXplProtocol() const
   {
      return m_xplProtocolWriting;
   }

   const boost::shared_ptr<shared::CDataContainer>& CDeviceIdentifier::getInnerDetails() const
   {
      return m_innerDetails;
   }
} //namespace xplrules


