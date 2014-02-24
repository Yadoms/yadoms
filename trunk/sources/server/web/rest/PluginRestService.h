#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"

class CPluginRestService : public IRestService
{
public:
   CPluginRestService(boost::shared_ptr<database::IDataProvider> dataProvider);
   virtual ~CPluginRestService();

public:
    // IRestService implementation
   virtual void configureDispatcher(CRestDispatcher & dispatcher);
   // [END] IRestService implementation

   const std::string & getRestKeyword();
   CJson getOnePlugin(const std::vector<std::string> & parameters, const CJson & requestContent);   
   CJson getAllPlugins(const std::vector<std::string> & parameters, const CJson & requestContent);  
   CJson createPlugin(const std::vector<std::string> & parameters, const CJson & requestContent);  
   CJson updatePlugin(const std::vector<std::string> & parameters, const CJson & requestContent);  
   CJson deletePlugin(const std::vector<std::string> & parameters, const CJson & requestContent);  
   CJson deleteAllPlugins(const std::vector<std::string> & parameters, const CJson & requestContent);  
   
   CJson transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string> & parameters, const CJson & requestContent);

private:
   boost::shared_ptr<database::IDataProvider> m_dataProvider;
   std::string m_restKeyword;
};