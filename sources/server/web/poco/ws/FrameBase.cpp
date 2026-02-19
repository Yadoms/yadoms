#include "stdafx.h"
#include "FrameBase.h"

namespace web
{
   namespace poco
   {
      namespace ws
      {
         const std::string CFrameBase::TypeFieldName = "type";
         const std::string CFrameBase::DataFieldName = "data";

         DECLARE_ENUM_IMPLEMENTATION_NESTED(CFrameBase::EFrameType, EFrameType,
                                            ((AcquisitionFilter))
                                            ((AcquisitionUpdate))
                                            ((AcquisitionSummaryUpdate))
                                            ((DeviceNew))
                                            ((LogEventNew))
                                            ((TaskUpdateNotification))
                                            ((IsAlive))
                                            ((TimeNotification))
                                            ((KeywordDeleted))
                                            ((DeviceDeleted))
                                            ((KeywordNew))
                                            ((DeviceBlackListed))
         );


         CFrameBase::CFrameBase(const std::string& content)
            : m_internalContainer(content)
         {
         }

         CFrameBase::CFrameBase(const EFrameType& type)
         {
            m_internalContainer.set<EFrameType>(TypeFieldName, type);
         }

         CFrameBase::EFrameType CFrameBase::getType() const
         {
            return m_internalContainer.get<EFrameType>(TypeFieldName);
         }

         const std::string& CFrameBase::getTypeFieldName()
         {
            return TypeFieldName;
         }

         std::string CFrameBase::serialize() const
         {
            return m_internalContainer.serialize();
         }
      } //namespace ws
   } //namespace poco
} //namespace web
