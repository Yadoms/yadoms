#include "stdafx.h"
#include "Maintenance.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/RestDispatcher.h"
#include "web/rest/Result.h"
#include "task/backup/Backup.h"
#include "task/exportData/ExportData.h"
#include "task/packLogs/PackLogs.h"
#include <shared/Log.h>
#include <boost/date_time/c_local_time_adjustor.hpp>

namespace web
{
   namespace rest
   {
      namespace service
      {
         std::string CMaintenance::m_restKeyword = std::string("maintenance");

         CMaintenance::CMaintenance(boost::shared_ptr<const IPathProvider> pathProvider,
                                    boost::shared_ptr<database::IDatabaseRequester> databaseRequester,
                                    boost::shared_ptr<database::IAcquisitionRequester> acquisitionRequester,
                                    boost::shared_ptr<task::CScheduler> taskScheduler)
            : m_pathProvider(pathProvider),
              m_databaseRequester(databaseRequester),
              m_acquisitionRequester(acquisitionRequester),
              m_taskScheduler(taskScheduler)
         {
         }


         void CMaintenance::configureDispatcher(CRestDispatcher& dispatcher)
         {
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("information"), CMaintenance::getDatabaseInformation);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("backup"), CMaintenance::getBackups);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("backup"), CMaintenance::startBackup);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "DELETE", (m_restKeyword)("backup")("*"), CMaintenance::deleteBackup);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "DELETE", (m_restKeyword)("backup"), CMaintenance::deleteAllBackups);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("packlogs"), CMaintenance::startPackLogs);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("logs"), CMaintenance::getLogs);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "DELETE", (m_restKeyword)("logs"), CMaintenance::deleteAllLogs);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("startExportData")("*"), CMaintenance::startExportData);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("exportData"), CMaintenance::getExportData);
         }


         const std::string& CMaintenance::getRestKeyword()
         {
            return m_restKeyword;
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CMaintenance::getDatabaseInformation(const std::vector<std::string>& parameters,
                                                                                                          const std::string& requestContent) const
         {
            try
            {
               auto result = m_databaseRequester->getInformation();
               result.set("backupSupport", m_databaseRequester->backupSupported());
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
                  const boost::shared_ptr<task::ITask> task(boost::make_shared<task::backup::CBackup>(m_pathProvider, m_databaseRequester));

                  std::string taskUid;
                  if (m_taskScheduler->runTask(task, taskUid))
                     YADOMS_LOG(information) << "Task : " << task->getName() << " successfully started. TaskId = " << taskUid;
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

                  if (!backup.empty() && boost::filesystem::exists(backup) && boost::filesystem::is_directory(backup))
                  {
                     std::vector<shared::CDataContainer> files;
                     // Check all subdirectories in plugin path
                     for (boost::filesystem::directory_iterator i(backup); i != boost::filesystem::directory_iterator(); ++i)
                     {
                        if (boost::filesystem::is_regular_file(i->path()))
                        {
                           if (!boost::starts_with(i->path().filename().string(), "backup_"))
                              continue;

                           auto fileSize = boost::filesystem::file_size(i->path());

                           const auto lastWriteTimeT = boost::filesystem::last_write_time(i->path());
                           auto lastWriteTimePosix = boost::date_time::c_local_adjustor<boost::posix_time::ptime>::utc_to_local(
                              boost::posix_time::from_time_t(lastWriteTimeT));

                           shared::CDataContainer file;
                           file.set("size", fileSize);
                           file.set("modificationDate", lastWriteTimePosix);
                           file.set("path", i->path().string());
                           file.set("url", i->path().filename().string());
                           file.set("inprogress", boost::iends_with(i->path().filename().string(), ".inprogress"));
                           files.push_back(file);
                        }
                     }

                     shared::CDataContainer result;
                     result.set("backups", files);
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
                  if (boost::filesystem::exists(backup / urlToDelete))
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
               for (boost::filesystem::directory_iterator i(backupPath); i != boost::filesystem::directory_iterator(); ++i)
               {
                  if (boost::filesystem::is_regular_file(i->path()))
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

         boost::shared_ptr<shared::serialization::IDataSerializable> CMaintenance::startPackLogs(const std::vector<std::string>& parameters,
                                                                                                 const std::string& requestContent)
         {
            try
            {
               const boost::shared_ptr<task::ITask> task(boost::make_shared<task::packLogs::CPackLogs>(m_pathProvider));

               std::string taskUid;
               if (m_taskScheduler->runTask(task, taskUid))
                  YADOMS_LOG(information) << "Task : " << task->getName() << " successfully started. TaskId = " << taskUid;
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

               if (!backupPath.empty() && boost::filesystem::exists(backupPath) && boost::filesystem::is_directory(backupPath))
               {
                  for (boost::filesystem::directory_iterator i(backupPath); i != boost::filesystem::directory_iterator(); ++i)
                  {
                     if (i->path().filename().string() != "logs.zip")
                        continue;

                     if (boost::filesystem::is_regular_file(i->path()))
                     {
                        const auto fileSize = boost::filesystem::file_size(i->path());

                        const auto lastWriteTimeT = boost::filesystem::last_write_time(i->path());
                        const auto lastWriteTimePosix = boost::date_time::c_local_adjustor<boost::posix_time::ptime>::utc_to_local(
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
               for (boost::filesystem::directory_iterator i(backupPath); i != boost::filesystem::directory_iterator(); ++i)
               {
                  if (boost::filesystem::is_regular_file(i->path()))
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
                                                                                                              m_acquisitionRequester,
                                                                                                              keywordId));

                  std::string taskUid;
                  if (m_taskScheduler->runTask(task, taskUid))
                     YADOMS_LOG(information) << "Task : " << task->getName() << " successfully started. TaskId = " << taskUid;
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

               if (!backupPath.empty() && boost::filesystem::exists(backupPath) && boost::filesystem::is_directory(backupPath))
               {
                  for (boost::filesystem::directory_iterator i(backupPath); i != boost::filesystem::directory_iterator(); ++i)
                  {
                     if (i->path().filename().string() != "exportData.zip")
                        continue;

                     if (boost::filesystem::is_regular_file(i->path()))
                     {
                        const auto fileSize = boost::filesystem::file_size(i->path());

                        const auto lastWriteTimeT = boost::filesystem::last_write_time(i->path());
                        const auto lastWriteTimePosix = boost::date_time::c_local_adjustor<boost::posix_time::ptime>::utc_to_local(
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
