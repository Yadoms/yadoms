#include "stdafx.h"
#include "WebsocketOnNewAcquisitionHandler.h"

namespace web
{
   namespace oatppServer
   {
      CWebsocketOnNewAcquisitionHandler::CWebsocketOnNewAcquisitionHandler(std::function<void(const std::string&)> sendOnSocketFct)
         : m_sendOnSocketFct(std::move(sendOnSocketFct))
      {
         m_newAcquisitionContainer.set("type", "newAcquisition");
      }

      void CWebsocketOnNewAcquisitionHandler::onNewAcquisition(boost::shared_ptr<database::entities::CAcquisition> acquisition)
      {
         if (std::find(m_keywords.begin(),
                       m_keywords.end(),
                       acquisition->KeywordId()) == m_keywords.end())
            return;
         
         m_newAcquisitionContainer.set("data", *acquisition);
         m_sendOnSocketFct(m_newAcquisitionContainer.serialize());
      }

      void CWebsocketOnNewAcquisitionHandler::setFilter(const std::vector<int>& keywords)
      {
         m_keywords = keywords;
      }
   } //namespace oatppServer
} //namespace web
