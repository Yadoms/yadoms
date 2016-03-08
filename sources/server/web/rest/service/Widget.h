#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"
#include "web/rest/RestDispatcher.h"

namespace web { namespace rest { namespace service {

   class CWidget : public IRestService
   {
   public:
      explicit CWidget(boost::shared_ptr<database::IDataProvider> dataProvider, const std::string & webServerPath);
      virtual ~CWidget();

   public:
      // IRestService implementation
      virtual void configureDispatcher(CRestDispatcher & dispatcher);
      // [END] IRestService implementation


      static const std::string & getRestKeyword();

   public:
      shared::CDataContainer getOneWidget(const std::vector<std::string> & parameters, const std::string & requestContent);
      shared::CDataContainer getAllWidgets(const std::vector<std::string> & parameters, const std::string & requestContent);
      shared::CDataContainer addWidget(const std::vector<std::string> & parameters, const std::string & requestContent);
      shared::CDataContainer deleteOneWidget(const std::vector<std::string> & parameters, const std::string & requestContent);
      shared::CDataContainer deleteAllWidgets(const std::vector<std::string> & parameters, const std::string & requestContent);
      shared::CDataContainer replaceAllWidgets(const std::vector<std::string> & parameters, const std::string & requestContent);
      shared::CDataContainer updateOneWidget(const std::vector<std::string> & parameters, const std::string & requestContent);
      shared::CDataContainer findWidgetPackages(const std::vector<std::string> & parameters, const std::string & requestContent);

      shared::CDataContainer transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string> & parameters, const std::string & requestContent);
   private:
      boost::shared_ptr<database::IDataProvider> m_dataProvider;
      static std::string m_restKeyword;
      std::string m_webServerPath;
   };


} //namespace service
} //namespace rest
} //namespace web 
