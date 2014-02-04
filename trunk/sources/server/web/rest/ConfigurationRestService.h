#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"


class CConfigurationRestService : public IRestService
{
public:
   CConfigurationRestService(boost::shared_ptr<IDataProvider> dataProvider);
   virtual ~CConfigurationRestService();

public:
   // IRestService implementation
   virtual void configureDispatcher(CRestDispatcher & dispatcher);
   // [END] IRestService implementation
   
   
   const std::string & getRestKeyword();

public:
   CJson getConfiguration(const std::vector<std::string> & parameters);
   CJson getSectionConfigurations(const std::vector<std::string> & parameters);
   CJson getAllConfigurations(const std::vector<std::string> & parameters);

private:
   boost::shared_ptr<IDataProvider> m_dataProvider;
   std::string m_restKeyword;
};