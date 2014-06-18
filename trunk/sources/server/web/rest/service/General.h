#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"
#include "web/rest/RestDispatcher.h"
#include "System.h"

namespace web { namespace rest { namespace service {

   class CGeneral : public IRestService
   {
   public:
      CGeneral(boost::shared_ptr<CSystem> systemInformation);
      virtual ~CGeneral();

   public:
      // IRestService implementation
      virtual void configureDispatcher(CRestDispatcher & dispatcher);
      // [END] IRestService implementation

      const std::string & getRestKeyword();

   public:
      web::rest::json::CJson getSerialPorts(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);
      web::rest::json::CJson getSystemInformation(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);

   private:
      static std::string m_restKeyword;

      boost::shared_ptr<CSystem> m_systemInformation;
   };


} //namespace service
} //namespace rest
} //namespace web 
