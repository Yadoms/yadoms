#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"
#include "RestDispatcher.h"

class CWidgetRestService : public IRestService
{
public:
   CWidgetRestService(boost::shared_ptr<database::IDataProvider> dataProvider, const std::string & webServerPath);
   virtual ~CWidgetRestService();

public:
   // IRestService implementation
   virtual void configureDispatcher(CRestDispatcher & dispatcher);
   // [END] IRestService implementation
   
   
   static const std::string & getRestKeyword();

public:
   CJson getOneWidget(const std::vector<std::string> & parameters, const CJson & requestContent);
   CJson getAllWidgets(const std::vector<std::string> & parameters, const CJson & requestContent);
   CJson getWidgetAcquisitions(const std::vector<std::string> & parameters, const CJson & requestContent);
   CJson addWidget(const std::vector<std::string> & parameters, const CJson & requestContent);
   CJson deleteOneWidget(const std::vector<std::string> & parameters, const CJson & requestContent);
   CJson deleteAllWidgets(const std::vector<std::string> & parameters, const CJson & requestContent);
   CJson replaceAllWidgets(const std::vector<std::string> & parameters, const CJson & requestContent);
   CJson updateOneWidget(const std::vector<std::string> & parameters, const CJson & requestContent);
   CJson findWidgetPackages(const std::vector<std::string> & parameters, const CJson & requestContent);
   
   CJson transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string> & parameters, const CJson & requestContent);
private:
   boost::shared_ptr<database::IDataProvider> m_dataProvider;
   static std::string m_restKeyword;
   std::string m_webServerPath;
};