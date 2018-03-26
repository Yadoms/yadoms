#pragma once

#include "IRestService.h"
#include "database/IDatabaseRequester.h"
#include "web/rest/RestDispatcher.h"
#include "task/Scheduler.h"
#include "IPathProvider.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CMaintenance : public IRestService
         {
         public:
            explicit CMaintenance(boost::shared_ptr<const IPathProvider> pathProvider, boost::shared_ptr<database::IDatabaseRequester> databaseRequester, boost::shared_ptr<task::CScheduler> taskScheduler);
            virtual ~CMaintenance();

            // IRestService implementation
            void configureDispatcher(CRestDispatcher& dispatcher) override;
            // [END] IRestService implementation

            static const std::string& getRestKeyword();

            boost::shared_ptr<shared::serialization::IDataSerializable> getDatabaseInformation(const std::vector<std::string>& parameters, const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> startBackup(const std::vector<std::string>& parameters, const std::string& requestContent);
            boost::shared_ptr<shared::serialization::IDataSerializable> getBackups(const std::vector<std::string>& parameters, const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> deleteBackup(const std::vector<std::string>& parameters, const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> deleteAllBackups(const std::vector<std::string>& parameters, const std::string& requestContent) const;

         private:
            static std::string m_restKeyword;
            boost::shared_ptr<const IPathProvider> m_pathProvider;
            boost::shared_ptr<database::IDatabaseRequester> m_databaseRequester;
            boost::shared_ptr<task::CScheduler> m_taskScheduler;
         };
      } //namespace service
   } //namespace rest
} //namespace web 


