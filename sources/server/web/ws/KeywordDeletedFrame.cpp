#include "stdafx.h"
#include "KeywordDeletedFrame.h"

namespace web
{
   namespace ws
   {
      const std::string CKeywordDeletedFrame::m_keyword = "keyword";

      CKeywordDeletedFrame::CKeywordDeletedFrame(boost::shared_ptr<const database::entities::CKeyword> content)
         : CFrameBase(EFrameType::kKeywordDeleted)
      {
         shared::CDataContainer local;
         local.set(m_keyword, content);
         m_internalContainer.set(m_dataFieldName, local);
      }

      CKeywordDeletedFrame::~CKeywordDeletedFrame()
      {
      }
   } //namespace ws
} //namespace web


