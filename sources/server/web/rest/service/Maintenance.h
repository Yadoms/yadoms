#pragma once

#include "IRestService.h"
#include "database/IDatabaseRequester.h"
#include "web/rest/RestDispatcher.h"

namespace web { namespace rest { namespace service {

   class CMaintenance : public IRestService
   {
   public:
      explicit CMaintenance(boost::shared_ptr<database::IDatabaseRequester> databaseRequester);
      virtual ~CMaintenance();

   public:
      // IRestService implementation
      virtual void configureDispatcher(CRestDispatcher & dispatcher);
      // [END] IRestService implementation

      const std::string & getRestKeyword();

   public:
      shared::CDataContainer getDatabaseInformation(const std::vector<std::string> & parameters, const std::string & requestContent);
      shared::CDataContainer startDatabaseBackup(const std::vector<std::string> & parameters, const std::string & requestContent);

   private:
      static std::string m_restKeyword;

     boost::shared_ptr<database::IDatabaseRequester> m_databaseRequester;
   };


} //namespace service
} //namespace rest
} //namespace web 
