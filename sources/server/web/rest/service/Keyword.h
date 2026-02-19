#pragma once
#include "IRestService.h"
#include "database/IDataProvider.h"
#include "communication/ISendMessageAsync.h"
#include "pluginSystem/Manager.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CKeyword final : public IRestService
         {
         public:
            CKeyword(boost::shared_ptr<database::IDataProvider> dataProvider,
                     boost::shared_ptr<dataAccessLayer::IKeywordManager> keywordManager,
                     communication::ISendMessageAsync& messageSender);
            ~CKeyword() override = default;

            // IRestService implementation
            void configurePocoDispatcher(poco::CRestDispatcher& dispatcher) override{}
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> endPoints() override;
            // [END] IRestService implementation

         private:
            boost::shared_ptr<IAnswer> getKeywordsV2(const boost::shared_ptr<IRequest>& request) const;
            boost::shared_ptr<IAnswer> getKeywordsAcquisitionsV2(const boost::shared_ptr<IRequest>& request) const;
            boost::shared_ptr<IAnswer> updateKeywordV2(const boost::shared_ptr<IRequest>& request) const;
            boost::shared_ptr<IAnswer> sendCommandV2(const boost::shared_ptr<IRequest>& request) const;

            //-----------------------------------------
            ///\brief   Data provider
            //-----------------------------------------
            boost::shared_ptr<database::IDataProvider> m_dataProvider;

            //-----------------------------------------
            ///\brief   Keyword manager
            //-----------------------------------------
            boost::shared_ptr<dataAccessLayer::IKeywordManager> m_keywordManager;
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> m_endPoints;

            //-----------------------------------------
            ///\brief   The rest keyword which identifies this rule
            //-----------------------------------------
            static std::string m_restKeyword;

            //-----------------------------------------
            ///\brief   The send message interface
            //-----------------------------------------
            communication::ISendMessageAsync& m_messageSender;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
