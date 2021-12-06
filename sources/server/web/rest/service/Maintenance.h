#pragma once
#include "IRestService.h"
#include "task/Scheduler.h"
#include "IPathProvider.h"
#include "database/IAcquisitionRequester.h"
#include "database/IDatabaseRequester.h"
#include "database/IDataProvider.h"
#include "database/IKeywordRequester.h"
#include "IUploadFileManager.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CMaintenance final : public IRestService
         {
         public:
            explicit CMaintenance(boost::shared_ptr<const IPathProvider> pathProvider,
                                  const boost::shared_ptr<database::IDataProvider>& dataProvider,
                                  boost::shared_ptr<task::CScheduler> taskScheduler,
                                  boost::shared_ptr<IUploadFileManager> uploadFileManager);
            ~CMaintenance() override = default;

            // IRestService implementation
            void configurePocoDispatcher(poco::CRestDispatcher& dispatcher) override;
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> endPoints() override;
            // [END] IRestService implementation

         private:
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
            boost::shared_ptr<shared::serialization::IDataSerializable> transactionalMethod(poco::CRestDispatcher::CRestMethodHandler realMethod,
                                                                                            const std::vector<std::string>& parameters,
                                                                                            const std::string& requestContent) const;

            boost::shared_ptr<IAnswer> getBackupsV2(const boost::shared_ptr<IRequest>& request) const;
            boost::shared_ptr<IAnswer> createBackupsV2(const boost::shared_ptr<IRequest>& request) const;

            boost::shared_ptr<std::string> fileUploadChunkRead(const std::string& requestContent) const;
            std::string fileUploadChunkReadGuid(const std::string& requestContent) const;
            std::string fileUploadChunkReadFilename(const std::string& requestContent) const;
            unsigned int fileUploadChunkReadFileSize(const std::string& requestContent) const;

            static std::string m_restKeyword;
            boost::shared_ptr<const IPathProvider> m_pathProvider;
            boost::shared_ptr<database::IDataProvider> m_dataProvider;
            boost::shared_ptr<database::IDatabaseRequester> m_databaseRequester;
            boost::shared_ptr<database::IKeywordRequester> m_keywordRequester;
            boost::shared_ptr<database::IAcquisitionRequester> m_acquisitionRequester;
            boost::shared_ptr<task::CScheduler> m_taskScheduler;
            boost::shared_ptr<IUploadFileManager> m_uploadFileManager;
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> m_endPoints;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
