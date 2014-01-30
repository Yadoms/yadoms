#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"

class CAcquisitionRestService : public IRestService
{
public:
   CAcquisitionRestService(boost::shared_ptr<IDataProvider> dataProvider);
   virtual ~CAcquisitionRestService();

public:
    // IRestService implementation
   virtual void configureDispatcher(CRestDispatcher & dispatcher);
   // [END] IRestService implementation

   const std::string & getRestKeyword();
   CJson getOneAcquisition(const std::vector<std::string> & parameters);   
   CJson getAllAcquisitions(const std::vector<std::string> & parameters);  
   
private:
   boost::shared_ptr<IDataProvider> m_dataProvider;
   std::string m_restKeyword;
};