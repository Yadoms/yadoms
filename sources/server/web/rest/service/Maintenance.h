#pragma once
#include "IRestService.h"
#include "web/rest/RestDispatcher.h"
#include "task/Scheduler.h"
#include "IPathProvider.h"
#include "database/IAcquisitionRequester.h"
#include "database/IDatabaseRequester.h"
#include "database/IDataProvider.h"
#include "database/IKeywordRequester.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CMaintenance : public IRestService
         {
         public:
            explicit CMaintenance(boost::shared_ptr<const IPathProvider> pathProvider,
                                  const boost::shared_ptr<database::IDataProvider>& dataProvider,
                                  boost::shared_ptr<task::CScheduler> taskScheduler);
            virtual ~CMaintenance() = default;

            // IRestService implementation
            void configureDispatcher(CRestDispatcher& dispatcher) override;
            // [END] IRestService implementation

            static const std::string& getRestKeyword();

            boost::shared_ptr<shared::serialization::IDataSerializable> getDatabaseInformation(const std::vector<std::string>& parameters,
                                                                                               const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> startBackup(const std::vector<std::string>& parameters,
                                                                                    const std::string& requestContent);
            boost::shared_ptr<shared::serialization::IDataSerializable> getBackups(const std::vector<std::string>& parameters,
                                                                                   const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> deleteBackup(const std::vector<std::string>& parameters,
                                                                                     const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> restoreBackup(const std::vector<std::string>& parameters,
                                                                                      const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> deleteAllBackups(const std::vector<std::string>& parameters,
                                                                                         const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> uploadBackup(const std::vector<std::string>& parameters,
                                                                                     const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> startPackLogs(const std::vector<std::string>& parameters,
                                                                                      const std::string& requestContent);
            boost::shared_ptr<shared::serialization::IDataSerializable> getLogs(const std::vector<std::string>& parameters,
                                                                                const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> deleteAllLogs(const std::vector<std::string>& parameters,
                                                                                      const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> startExportData(const std::vector<std::string>& parameters,
                                                                                        const std::string& requestContent);
            boost::shared_ptr<shared::serialization::IDataSerializable> getExportData(const std::vector<std::string>& parameters,
                                                                                      const std::string& requestContent) const;

         private:
            boost::shared_ptr<shared::serialization::IDataSerializable> transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod,
                                                                                            const std::vector<std::string>& parameters,
                                                                                            const std::string& requestContent) const;

            static std::string m_restKeyword;
            boost::shared_ptr<const IPathProvider> m_pathProvider;
            boost::shared_ptr<database::IDataProvider> m_dataProvider;
            boost::shared_ptr<database::IDatabaseRequester> m_databaseRequester;
            boost::shared_ptr<database::IKeywordRequester> m_keywordRequester;
            boost::shared_ptr<database::IAcquisitionRequester> m_acquisitionRequester;
            boost::shared_ptr<task::CScheduler> m_taskScheduler;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
