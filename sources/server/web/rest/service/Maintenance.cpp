#include "stdafx.h"
#include "Maintenance.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/RestDispatcher.h"
#include "web/rest/Result.h"
#include "task/backup/Backup.h"
#include "task/backup/Restore.h"
#include "task/exportData/ExportData.h"
#include "task/packLogs/PackLogs.h"
#include <shared/Log.h>
#include <shared/encryption/Base64.h>
#include <shared/http/HttpHelpers.h>
#include <boost/date_time/c_local_time_adjustor.hpp>
#include <regex>
#include <utility>


namespace web
{
   namespace rest
   {
      namespace service
      {
         std::string CMaintenance::m_restKeyword = std::string("maintenance");

         CMaintenance::CMaintenance(boost::shared_ptr<const IPathProvider> pathProvider,
                                    const boost::shared_ptr<database::IDataProvider>& dataProvider,
                                    boost::shared_ptr<task::CScheduler> taskScheduler,
                                    boost::shared_ptr<IUploadFileManager> uploadFileManager)
            : m_pathProvider(std::move(pathProvider)),
              m_dataProvider(dataProvider),
              m_databaseRequester(dataProvider->getDatabaseRequester()),
              m_keywordRequester(dataProvider->getKeywordRequester()),
              m_acquisitionRequester(dataProvider->getAcquisitionRequester()),
              m_taskScheduler(std::move(taskScheduler)),
              m_uploadFileManager(uploadFileManager)
         {
         }


         void CMaintenance::configurePocoDispatcher(CRestDispatcher& dispatcher)
         {
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("information"), CMaintenance::getDatabaseInformation);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("backup"), CMaintenance::getBackups);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("backup"), CMaintenance::startBackup);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "DELETE", (m_restKeyword)("backup")("*"), CMaintenance::deleteBackup);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("restore")("*"), CMaintenance::restoreBackup);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "DELETE", (m_restKeyword)("backup"), CMaintenance::deleteAllBackups);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("uploadBackup"), CMaintenance::uploadBackup);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("packlogs"), CMaintenance::startPackLogs);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("logs"), CMaintenance::getLogs);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "DELETE", (m_restKeyword)("logs"), CMaintenance::deleteAllLogs);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("startExportData")("*"), CMaintenance::startExportData);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("exportData"), CMaintenance::getExportData);
         }

         boost::shared_ptr<std::vector<boost::shared_ptr<IRestAccessPoint>>> CMaintenance::accessPoints()
         {
            if (m_accessPoints != nullptr)
               return m_accessPoints;

            m_accessPoints = boost::make_shared<std::vector<boost::shared_ptr<IRestAccessPoint>>>();
            //TODO

            return m_accessPoints;
         }


         const std::string& CMaintenance::getRestKeyword()
         {
            return m_restKeyword;
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CMaintenance::transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod,
                                                                                                       const std::vector<std::string>& parameters,
                                                                                                       const std::string& requestContent) const
         {
            auto pTransactionalEngine = m_dataProvider->getTransactionalEngine();
            boost::shared_ptr<shared::serialization::IDataSerializable> result;
            try
            {
               if (pTransactionalEngine)
                  pTransactionalEngine->transactionBegin();
               result = realMethod(parameters, requestContent);
            }
            catch (std::exception& ex)
            {
               result = CResult::GenerateError(ex);
            }
            catch (...)
            {
               result = CResult::GenerateError("unknown exception plugin rest method");
            }

            if (pTransactionalEngine)
            {
               if (CResult::isSuccess(*boost::dynamic_pointer_cast<shared::CDataContainer>(result)))
                  pTransactionalEngine->transactionCommit();
               else
                  pTransactionalEngine->transactionRollback();
            }
            return result;
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CMaintenance::getDatabaseInformation(const std::vector<std::string>& parameters,
                                                                                                          const std::string& requestContent) const
         {
            try
            {
               auto result = m_databaseRequester->getInformation();
               result->set("backupSupport", m_databaseRequester->backupSupported());
               return CResult::GenerateSuccess(result);
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retrieving database information");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CMaintenance::startBackup(const std::vector<std::string>& parameters,
                                                                                               const std::string& requestContent)
         {
            try
            {
               if (m_databaseRequester->backupSupported())
               {
                  const boost::shared_ptr<task::ITask> task(
                     boost::make_shared<task::backup::CBackup>(m_pathProvider, m_databaseRequester));

                  std::string taskUid;
                  if (m_taskScheduler->runTask(task, taskUid))
                     YADOMS_LOG(information) << "Task : " << task->getName() << " successfully started. TaskId = " <<
                        taskUid;
                  else
                     YADOMS_LOG(error) << "Task : " << task->getName() << " fail to start";


                  shared::CDataContainer result;
                  result.set("taskId", taskUid);
                  return CResult::GenerateSuccess(result);
               }
               return CResult::GenerateError("backup not supported");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in starting backup");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CMaintenance::getBackups(const std::vector<std::string>& parameters,
                                                                                              const std::string& requestContent) const
         {
            try
            {
               if (m_databaseRequester->backupSupported())
               {
                  auto backup = m_pathProvider->backupPath();

                  if (!backup.empty() && exists(backup) && is_directory(backup))
                  {
                     shared::CDataContainer result;
                     result.createArray("backups");
                     // Check all subdirectories in plugin path
                     for (boost::filesystem::directory_iterator i(backup); i != boost::filesystem::directory_iterator();
                          ++i)
                     {
                        if (is_regular_file(i->path()))
                        {
                           if (!boost::starts_with(i->path().filename().string(), "backup_"))
                              continue;

                           auto fileSize = file_size(i->path());

                           const auto lastWriteTimeT = last_write_time(i->path());
                           auto lastWriteTimePosix = boost::date_time::c_local_adjustor<boost::posix_time::ptime>::
                              utc_to_local(
                                 boost::posix_time::from_time_t(lastWriteTimeT));

                           shared::CDataContainer file;
                           file.set("size", fileSize);
                           file.set("modificationDate", lastWriteTimePosix);
                           file.set("path", i->path().string());
                           file.set("url", i->path().filename().string());
                           file.set("inprogress", boost::iends_with(i->path().filename().string(), ".inprogress"));
                           result.appendArray("backups", file);
                        }
                     }

                     return CResult::GenerateSuccess(result);
                  }
                  //backup do not exists
                  return CResult::GenerateError();
               }
               return CResult::GenerateError("backup not supported");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retrieving last backup data");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CMaintenance::deleteBackup(const std::vector<std::string>& parameters,
                                                                                                const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 2)
               {
                  const auto urlToDelete = parameters[2];

                  const auto backup = m_pathProvider->backupPath();
                  if (exists(backup / urlToDelete))
                  {
                     boost::system::error_code ec;
                     if (boost::filesystem::remove(backup / urlToDelete, ec))
                        return CResult::GenerateSuccess();
                     return CResult::GenerateError(ec.message());
                  }
                  //backup do not exists
                  return CResult::GenerateError("file do not exists");
               }

               return CResult::GenerateError("invalid parameter. Can not retrieve file to delete");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in deleting backup data");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CMaintenance::restoreBackup(const std::vector<std::string>& parameters,
                                                                                                 const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 2)
               {
                  const auto backupFileName = parameters[2];

                  const boost::shared_ptr<task::ITask> task(boost::make_shared<task::backup::CRestore>(backupFileName,
                                                                                                       m_pathProvider));

                  std::string taskUid;
                  if (!m_taskScheduler->runTask(task, taskUid))
                  {
                     YADOMS_LOG(error) << "Task : " << task->getName() << " fail to start";
                     return CResult::GenerateError("Task : " + task->getName() + " failed to start");
                  }

                  YADOMS_LOG(information) << "Task : " << task->getName() << " successfully started. TaskId = " <<
                     taskUid;

                  shared::CDataContainer result;
                  result.set("taskId", taskUid);
                  return CResult::GenerateSuccess(result);
               }

               return CResult::GenerateError("invalid parameter. Can not retrieve file to delete");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in deleting backup data");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CMaintenance::deleteAllBackups(const std::vector<std::string>& parameters,
                                                                                                    const std::string& requestContent) const
         {
            try
            {
               const auto backupPath = m_pathProvider->backupPath();

               boost::system::error_code ec;
               std::string errors;
               std::vector<shared::CDataContainer> files;
               // Check all subdirectories in plugin path
               for (boost::filesystem::directory_iterator i(backupPath); i != boost::filesystem::directory_iterator();
                    ++i)
               {
                  if (is_regular_file(i->path()))
                  {
                     if (!boost::starts_with(i->path().filename().string(), "backup_"))
                        continue;

                     if (!boost::filesystem::remove(i->path(), ec))
                        errors += ec.message() + '\n';
                  }
               }

               if (errors.empty())
                  return CResult::GenerateSuccess();
               return CResult::GenerateError(errors);
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in deleting backup data");
            }
         }

         boost::shared_ptr<std::string> CMaintenance::fileUploadChunkRead(const std::string& requestContent) const
         {
            static const std::string Base64Key(";base64,");
            const auto position = std::search(requestContent.begin(),
                                              requestContent.end(),
                                              Base64Key.begin(),
                                              Base64Key.end(),
                                              [](const auto c1, const auto c2)
                                              {
                                                 return std::tolower(c1) == std::tolower(c2);
                                              });
            if (position == requestContent.end())
               throw std::runtime_error("Invalid file");
            const auto dataStart = position + Base64Key.size();

            return boost::make_shared<std::string>(shared::encryption::CBase64::decode(std::string(dataStart, requestContent.end())));
         }

         std::string CMaintenance::fileUploadChunkReadGuid(const std::string& requestContent) const
         {
            const std::regex pattern("guid=([a-z0-9-]+)&",
                                     std::regex_constants::icase);
            std::smatch match;
            if (!std::regex_search(requestContent, match, pattern) || match.size() < 2)
               throw std::runtime_error("Fail to decode XHR file : no guid found");
            return match[1].str();
         }

         std::string CMaintenance::fileUploadChunkReadFilename(const std::string& requestContent) const
         {
            const std::regex pattern(R"(filename=([\w,\s\.-]*)&)",
                                     std::regex_constants::icase);
            std::smatch match;
            if (!std::regex_search(requestContent, match, pattern) || match.size() < 2)
               throw std::runtime_error("Fail to decode XHR file : no filename found");
            return match[1].str();
         }

         unsigned int CMaintenance::fileUploadChunkReadFileSize(const std::string& requestContent) const
         {
            const std::regex pattern(R"(filesize=([0-9]+)&)",
                                     std::regex_constants::icase);
            std::smatch match;
            if (!std::regex_search(requestContent, match, pattern) || match.size() < 2)
               throw std::runtime_error("Fail to decode XHR file : no file size found");
            return std::stoul(match[1].str());
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CMaintenance::uploadBackup(const std::vector<std::string>& parameters,
                                                                                                const std::string& requestContent) const
         {
            try
            {
               const auto decodedRequestContent = shared::http::CHttpHelpers::urlDecode(requestContent);

               const auto guid = fileUploadChunkReadGuid(requestContent);
               const auto filename = fileUploadChunkReadFilename(requestContent);
               const auto fileSize = fileUploadChunkReadFileSize(requestContent);

               const std::regex backupFilenamePattern(R"(^backup_[0-9]{8}T[0-9]{6}\.[0-9]+\.zip$)",
                                                      std::regex_constants::icase);
               std::smatch match;
               if (!std::regex_search(filename, match, backupFilenamePattern) || match.size() != 1)
                  throw std::runtime_error("Fail to decode XHR file : no valid filename found");

               const auto chunk = fileUploadChunkRead(decodedRequestContent);

               m_uploadFileManager->storeChunk(guid,
                                               m_pathProvider->backupPath() / filename,
                                               fileSize,
                                               chunk);

               return CResult::GenerateSuccess();
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in sending extra query to plugin");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CMaintenance::startPackLogs(const std::vector<std::string>& parameters,
                                                                                                 const std::string& requestContent)
         {
            try
            {
               const boost::shared_ptr<task::ITask> task(boost::make_shared<task::packLogs::CPackLogs>(m_pathProvider));

               std::string taskUid;
               if (m_taskScheduler->runTask(task, taskUid))
                  YADOMS_LOG(information) << "Task : " << task->getName() << " successfully started. TaskId = " <<
                     taskUid;
               else
                  YADOMS_LOG(error) << "Task : " << task->getName() << " fail to start";


               shared::CDataContainer result;
               result.set("taskId", taskUid);
               return CResult::GenerateSuccess(result);
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in packing logs");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CMaintenance::getLogs(const std::vector<std::string>& parameters,
                                                                                           const std::string& requestContent) const
         {
            try
            {
               auto backupPath = m_pathProvider->backupPath();

               if (!backupPath.empty() && exists(backupPath) && is_directory(backupPath))
               {
                  for (boost::filesystem::directory_iterator i(backupPath); i != boost::filesystem::directory_iterator()
                       ; ++i)
                  {
                     if (i->path().filename().string() != "logs.zip")
                        continue;

                     if (is_regular_file(i->path()))
                     {
                        const auto fileSize = file_size(i->path());

                        const auto lastWriteTimeT = last_write_time(i->path());
                        const auto lastWriteTimePosix = boost::date_time::c_local_adjustor<boost::posix_time::ptime>::
                           utc_to_local(
                              boost::posix_time::from_time_t(lastWriteTimeT));

                        shared::CDataContainer file;
                        file.set("size", fileSize);
                        file.set("modificationDate", lastWriteTimePosix);
                        file.set("path", i->path().string());
                        file.set("url", i->path().filename().string());
                        file.set("inprogress", boost::iends_with(i->path().filename().string(), ".inprogress"));
                        shared::CDataContainer result;
                        result.set("logs", file);
                        return CResult::GenerateSuccess(result);
                     }
                  }
               }
               // Logs don't exist
               return CResult::GenerateError();
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retrieving last logs data");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CMaintenance::deleteAllLogs(const std::vector<std::string>& parameters,
                                                                                                 const std::string& requestContent) const
         {
            try
            {
               const auto backupPath = m_pathProvider->backupPath();

               boost::system::error_code ec;
               std::string errors;
               for (boost::filesystem::directory_iterator i(backupPath); i != boost::filesystem::directory_iterator();
                    ++i)
               {
                  if (is_regular_file(i->path()))
                  {
                     if (i->path().filename().string() != "logs.zip")
                        continue;

                     if (!boost::filesystem::remove(i->path(), ec))
                        errors += ec.message() + '\n';
                  }
               }

               if (errors.empty())
                  return CResult::GenerateSuccess();
               return CResult::GenerateError(errors);
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in deleting backup data");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CMaintenance::startExportData(const std::vector<std::string>& parameters,
                                                                                                   const std::string& requestContent)
         {
            try
            {
               if (parameters.size() > 2)
               {
                  const auto keywordId = std::stoi(parameters[2]);

                  const boost::shared_ptr<task::ITask> task(boost::make_shared<task::exportData::CExportData>(m_pathProvider,
                                                                                                              m_keywordRequester,
                                                                                                              m_acquisitionRequester,
                                                                                                              keywordId));

                  std::string taskUid;
                  if (m_taskScheduler->runTask(task, taskUid))
                     YADOMS_LOG(information) << "Task : " << task->getName() << " successfully started. TaskId = " <<
                        taskUid;
                  else
                     YADOMS_LOG(error) << "Task : " << task->getName() << " fail to start";


                  shared::CDataContainer result;
                  result.set("taskId", taskUid);
                  return CResult::GenerateSuccess(result);
               }

               return CResult::GenerateError("invalid parameter. Can not retrieve file to delete");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in export data");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CMaintenance::getExportData(const std::vector<std::string>& parameters,
                                                                                                 const std::string& requestContent) const
         {
            try
            {
               auto backupPath = m_pathProvider->backupPath();

               if (!backupPath.empty() && exists(backupPath) && is_directory(backupPath))
               {
                  for (boost::filesystem::directory_iterator i(backupPath); i != boost::filesystem::directory_iterator()
                       ; ++i)
                  {
                     if (i->path().filename().string() != "exportData.zip")
                        continue;

                     if (is_regular_file(i->path()))
                     {
                        const auto fileSize = file_size(i->path());

                        const auto lastWriteTimeT = last_write_time(i->path());
                        const auto lastWriteTimePosix = boost::date_time::c_local_adjustor<boost::posix_time::ptime>::
                           utc_to_local(
                              boost::posix_time::from_time_t(lastWriteTimeT));

                        shared::CDataContainer file;
                        file.set("size", fileSize);
                        file.set("modificationDate", lastWriteTimePosix);
                        file.set("path", i->path().string());
                        file.set("url", i->path().filename().string());
                        file.set("inprogress", boost::iends_with(i->path().filename().string(), ".inprogress"));
                        shared::CDataContainer result;
                        result.set("exportData", file);
                        return CResult::GenerateSuccess(result);
                     }
                  }
               }
               // Logs don't exist
               return CResult::GenerateError();
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retrieving last exportData");
            }
         }
      } //namespace service
   } //namespace rest
} //namespace web 
