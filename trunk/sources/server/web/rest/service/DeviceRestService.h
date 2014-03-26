#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"

namespace web { namespace rest { namespace service {

   class CDeviceRestService : public IRestService
   {
   public:
      CDeviceRestService(boost::shared_ptr<database::IDataProvider> dataProvider);
      virtual ~CDeviceRestService();

   public:
      // IRestService implementation
      virtual void configureDispatcher(CRestDispatcher & dispatcher);
      // [END] IRestService implementation

      static const std::string & getRestKeyword();
      web::rest::json::CJson getOneDevice(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);   
      web::rest::json::CJson getAllDevices(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);  
      //web::rest::json::CJson getDeviceLastAcquisition(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);  

   private:
      boost::shared_ptr<database::IDataProvider> m_dataProvider;
      static std::string m_restKeyword;
   };


} //namespace service
} //namespace rest
} //namespace web 
