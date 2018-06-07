#include "stdafx.h"
#include "KeywordNewFrame.h"

namespace web
{
   namespace ws
   {
      const std::string CKeywordNewFrame::m_keyword = "keyword";

      CKeywordNewFrame::CKeywordNewFrame(boost::shared_ptr<const database::entities::CKeyword> content)
         : CFrameBase(EFrameType::kKeywordNew)
      {
         shared::CDataContainer local;
         local.set(m_keyword, content);
         m_internalContainer.set(m_dataFieldName, local);
      }

      CKeywordNewFrame::~CKeywordNewFrame()
      {
      }
   } //namespace ws
} //namespace web


