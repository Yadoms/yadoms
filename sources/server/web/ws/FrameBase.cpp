#include "stdafx.h"
#include "FrameBase.h"

namespace web
{
   namespace ws
   {
      const std::string CFrameBase::m_typeFieldName = "type";
      const std::string CFrameBase::m_dataFieldName = "data";

      DECLARE_ENUM_IMPLEMENTATION_NESTED(CFrameBase::EFrameType, EFrameType,
         ((AcquisitionFilter))
         ((AcquisitionUpdate))
         ((AcquisitionSummaryUpdate))
         ((DeviceNew))
         ((LogEventNew))
         ((TaskUpdateNotification))
         ((IsAlive))
         ((TimeNotification))
      );


      CFrameBase::CFrameBase(const std::string& content)
         : m_internalContainer(content)
      {
      }

      CFrameBase::CFrameBase(const EFrameType& type)
      {
         m_internalContainer.set<EFrameType>(m_typeFieldName, type);
      }

      CFrameBase::~CFrameBase()
      {
      }

      CFrameBase::EFrameType CFrameBase::getType()
      {
         return m_internalContainer.get<EFrameType>(m_typeFieldName);
      }

      const std::string& CFrameBase::getTypeFieldName()
      {
         return m_typeFieldName;
      }

      const std::string CFrameBase::serialize() const
      {
         return m_internalContainer.serialize();
      }
   } //namespace ws
} //namespace web


