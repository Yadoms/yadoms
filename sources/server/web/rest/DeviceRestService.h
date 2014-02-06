#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"

class CDeviceRestService : public IRestService
{
public:
   CDeviceRestService(boost::shared_ptr<IDataProvider> dataProvider);
   virtual ~CDeviceRestService();

public:
    // IRestService implementation
   virtual void configureDispatcher(CRestDispatcher & dispatcher);
   // [END] IRestService implementation

   const std::string & getRestKeyword();
   CJson getOneDevice(const std::vector<std::string> & parameters, const CJson & requestContent);   
   CJson getAllDevices(const std::vector<std::string> & parameters, const CJson & requestContent);  
   CJson getDeviceLastAcquisition(const std::vector<std::string> & parameters, const CJson & requestContent);  
   
private:
   boost::shared_ptr<IDataProvider> m_dataProvider;
   std::string m_restKeyword;
};