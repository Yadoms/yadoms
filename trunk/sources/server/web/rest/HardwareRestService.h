#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"

class CHardwareRestService : public IRestService
{
public:
   CHardwareRestService(boost::shared_ptr<IDataProvider> dataProvider);
   virtual ~CHardwareRestService();

public:
    // IRestService implementation
   virtual void configureDispatcher(CRestDispatcher & dispatcher);
   // [END] IRestService implementation

   const std::string & getRestKeyword();
   CJson getOneHardware(const std::vector<std::string> & parameters, const CJson & requestContent);   
   CJson getAllHardwares(const std::vector<std::string> & parameters, const CJson & requestContent);  
   
private:
   boost::shared_ptr<IDataProvider> m_dataProvider;
   std::string m_restKeyword;
};