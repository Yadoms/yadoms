#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"
#include "web/rest/RestDispatcher.h"
#include "SystemInformation.h"

namespace web { namespace rest { namespace service {

   class CSystem : public IRestService
   {
   public:
      CSystem(boost::shared_ptr<CSystemInformation> systemInformation);
      virtual ~CSystem();

   public:
      // IRestService implementation
      virtual void configureDispatcher(CRestDispatcher & dispatcher);
      // [END] IRestService implementation

      const std::string & getRestKeyword();

   public:
      shared::CDataContainer getBinding(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);
      shared::CDataContainer getSystemInformation(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);

   private:
      shared::CDataContainer getSerialPorts(const std::string & query);

   private:
      static std::string m_restKeyword;

      boost::shared_ptr<CSystemInformation> m_systemInformation;
   };


} //namespace service
} //namespace rest
} //namespace web 
