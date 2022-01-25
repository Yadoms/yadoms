#pragma once
#include "dataAccessLayer/observers/INewAcquisitionObserver.h"

namespace web
{
   namespace oatppServer
   {
      class CWebsocketOnNewAcquisitionHandler final : public dataAccessLayer::observers::INewAcquisitionObserver
      {
      public:
         explicit CWebsocketOnNewAcquisitionHandler(std::function<void(const std::string&)> sendOnSocketFct);
         ~CWebsocketOnNewAcquisitionHandler() override = default;

         // dataAccessLayer::observers::INewAcquisitionObserver Implementation
         void onNewAcquisition(boost::shared_ptr<database::entities::CAcquisition> acquisition) override;
         // [END] dataAccessLayer::observers::INewAcquisitionObserver Implementation

         void setFilter(const std::vector<int>& keywords);

      private:
         std::vector<int> m_keywords;
         std::function<void(const std::string&)> m_sendOnSocketFct;
         shared::CDataContainer m_newAcquisitionContainer;
      };
   } //namespace oatppServer
} //namespace web
