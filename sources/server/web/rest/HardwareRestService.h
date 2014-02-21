#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"

class CHardwareRestService : public IRestService
{
public:
   CHardwareRestService(boost::shared_ptr<server::database::IDataProvider> dataProvider);
   virtual ~CHardwareRestService();

public:
    // IRestService implementation
   virtual void configureDispatcher(CRestDispatcher & dispatcher);
   // [END] IRestService implementation

   static const std::string & getRestKeyword();
   CJson getOneHardware(const std::vector<std::string> & parameters, const CJson & requestContent);   
   CJson getAllHardwares(const std::vector<std::string> & parameters, const CJson & requestContent);  
   CJson createHardware(const std::vector<std::string> & parameters, const CJson & requestContent);  
   CJson updateHardware(const std::vector<std::string> & parameters, const CJson & requestContent);  
   CJson deleteHardware(const std::vector<std::string> & parameters, const CJson & requestContent);  
   CJson deleteAllHardwares(const std::vector<std::string> & parameters, const CJson & requestContent);  
   
   CJson transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string> & parameters, const CJson & requestContent);

private:
   boost::shared_ptr<server::database::IDataProvider> m_dataProvider;
   static std::string m_restKeyword;
};