#pragma once

#include "IRestService.h"
#include "database/IDatabaseRequester.h"
#include "web/rest/RestDispatcher.h"
#include "task/Scheduler.h"
#include "IPathProvider.h"

namespace web { namespace rest { namespace service {

   class CMaintenance : public IRestService
   {
   public:
      explicit CMaintenance(const IPathProvider& pathProvider, boost::shared_ptr<database::IDatabaseRequester> databaseRequester, boost::shared_ptr<task::CScheduler> taskScheduler);
      virtual ~CMaintenance();

   public:
      // IRestService implementation
      virtual void configureDispatcher(CRestDispatcher & dispatcher);
      // [END] IRestService implementation

      const std::string & getRestKeyword();

   public:
      shared::CDataContainer getDatabaseInformation(const std::vector<std::string> & parameters, const std::string & requestContent);
      shared::CDataContainer startBackup(const std::vector<std::string> & parameters, const std::string & requestContent);
      shared::CDataContainer getBackups(const std::vector<std::string> & parameters, const std::string & requestContent);
      shared::CDataContainer deleteBackup(const std::vector<std::string> & parameters, const std::string & requestContent);

   private:
      static std::string m_restKeyword;
      const IPathProvider& m_pathProvider;
      boost::shared_ptr<database::IDatabaseRequester> m_databaseRequester;
      boost::shared_ptr<task::CScheduler> m_taskScheduler;
   };


} //namespace service
} //namespace rest
} //namespace web 
