#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"
#include "web/rest/RestDispatcher.h"
#include "IRunningInformation.h"

namespace web { namespace rest { namespace service {

   class CSystem : public IRestService
   {
   public:
      explicit CSystem();
      virtual ~CSystem();

   public:
      // IRestService implementation
      virtual void configureDispatcher(CRestDispatcher & dispatcher);
      // [END] IRestService implementation

      const std::string & getRestKeyword();

      
   public:
      shared::CDataContainer getBinding(const std::vector<std::string> & parameters, const std::string & requestContent);
      shared::CDataContainer getSystemInformation(const std::vector<std::string> & parameters, const std::string & requestContent);

   private:
      shared::CDataContainer getSerialPorts();
      shared::CDataContainer getNetworkInterfaces(bool includeLoopback);
      shared::CDataContainer platformIs(const std::string& refPlatform) const;

   private:
      static std::string m_restKeyword;

	  boost::shared_ptr<IRunningInformation> m_runningInformation;
   };


} //namespace service
} //namespace rest
} //namespace web 
