#include <boost/date_time/c_local_time_adjustor.hpp>

#include "stdafx.h"
#include "Maintenance.h"
#include "RestEndPoint.h"
#include "task/backup/Backup.h"
#include "web/rest/AcceptedAnswer.h"
#include "web/rest/ErrorAnswer.h"
#include "web/rest/Helpers.h"
#include "web/rest/NoContentAnswer.h"
#include "web/rest/SuccessAnswer.h"


namespace web
{
   namespace rest
   {
      namespace service
      {
         boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> CMaintenance::endPoints()
         {
            if (m_endPoints != nullptr)
               return m_endPoints;

            m_endPoints = boost::make_shared<std::vector<boost::shared_ptr<IRestEndPoint>>>();

            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "maintenance/backups", getBackupsV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "maintenance/backups/{url}", getBackupsV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kPost, "maintenance/backups", createBackupsV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kDelete, "maintenance/backups", deleteBackupV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kDelete, "maintenance/backups/{url}", deleteBackupV2));


            //TODO            
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("restore")("*"), CMaintenance::restoreBackup)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("uploadBackup"), CMaintenance::uploadBackup)
            //
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("packlogs"), CMaintenance::startPackLogs)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("logs"), CMaintenance::getLogs)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "DELETE", (m_restKeyword)("logs"), CMaintenance::deleteAllLogs)
            //
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("startExportData")("*"), CMaintenance::startExportData)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("exportData"), CMaintenance::getExportData)

            return m_endPoints;
         }

         boost::shared_ptr<IAnswer> CMaintenance::getBackupsV2(const boost::shared_ptr<IRequest>& request)
         {
            try
            {
               if (!m_databaseRequester->backupSupported())
                  return boost::make_shared<CNoContentAnswer>();

               const auto url = request->pathVariable("url", std::string());

               const auto backupPath = m_pathProvider->backupPath();

               if (backupPath.empty() || !exists(backupPath) || !is_directory(backupPath))
                  return boost::make_shared<CNoContentAnswer>();

               shared::CDataContainer result;
               result.createArray("backups");
               for (boost::filesystem::directory_iterator backupDir(backupPath); backupDir != boost::filesystem::directory_iterator(); ++backupDir)
               {
                  if (!is_regular_file(backupDir->path()))
                     continue;

                  if (!boost::starts_with(backupDir->path().filename().string(), "backup_"))
                     continue;

                  auto fileSize = file_size(backupDir->path());

                  const auto lastWriteTimeT = last_write_time(backupDir->path());
                  const auto lastWriteTimePosix = boost::date_time::c_local_adjustor<boost::posix_time::ptime>::utc_to_local(
                     boost::posix_time::from_time_t(lastWriteTimeT));

                  shared::CDataContainer file;
                  file.set("size", fileSize);
                  file.set("modificationDate", lastWriteTimePosix);
                  file.set("path", backupDir->path().string());
                  file.set("url", backupDir->path().filename().string());
                  file.set("inprogress", boost::iends_with(backupDir->path().filename().string(), ".inprogress"));

                  if (!url.empty() && url == backupDir->path().filename().string())
                     return boost::make_shared<CSuccessAnswer>(file); // Get only one backup

                  result.appendArray("backups", file);
               }

               const auto backupInProgressTaskUid = backupInProgress();
               if (!backupInProgressTaskUid.empty())
                  result.set("inProgress", "tasks/" + backupInProgressTaskUid);

               if (result.arraySize("backups") == 0 && backupInProgressTaskUid.empty())
                  return boost::make_shared<CNoContentAnswer>();

               return boost::make_shared<CSuccessAnswer>(result);
            }
            catch (const shared::exception::COutOfRange& exception)
            {
               YADOMS_LOG(error) << "Error processing getBackups request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest);
            }
            catch (const std::exception& exception)
            {
               YADOMS_LOG(error) << "Error processing getBackups request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get backups");
            }
         }

         std::string CMaintenance::backupInProgress()
         {
            boost::lock_guard<boost::recursive_mutex> lock(m_backupInProgressTaskUidMutex);

            if (m_backupInProgressTaskUid.empty())
               return std::string();

            if (m_taskScheduler->getTask(m_backupInProgressTaskUid)->getStatus() != task::ETaskStatus::kStarted)
               m_backupInProgressTaskUid.clear();

            return m_backupInProgressTaskUid;
         }

         void CMaintenance::setBackupInProgress(const std::string& taskUid)
         {
            boost::lock_guard<boost::recursive_mutex> lock(m_backupInProgressTaskUidMutex);
            m_backupInProgressTaskUid = taskUid;
         }

         boost::shared_ptr<IAnswer> CMaintenance::createBackupsV2(const boost::shared_ptr<IRequest>& request)
         {
            boost::lock_guard<boost::recursive_mutex> lock(m_backupInProgressTaskUidMutex);
            try
            {
               if (!m_databaseRequester->backupSupported())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "backup not supported");

               if (!backupInProgress().empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "backup already in progress");

               const auto task(boost::make_shared<task::backup::CBackup>(m_pathProvider, m_databaseRequester));

               std::string taskUid;
               if (!m_taskScheduler->runTask(task, taskUid))
                  throw std::runtime_error("Task : " + task->getName() + " fail to start");
               setBackupInProgress(taskUid);

               YADOMS_LOG(information) << "Task : " << task->getName() << " successfully started. TaskId = " << taskUid;

               return CHelpers::createLongRunningOperationAnswer(taskUid);
            }
            catch (const std::exception& exception)
            {
               YADOMS_LOG(error) << "Fail to create backup : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to create backup");
            }
         }

         boost::shared_ptr<IAnswer> CMaintenance::deleteBackupV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               const auto url = request->pathVariable("url", std::string());

               if (!url.empty())
               {
                  // Delete one backup
                  const auto backup = m_pathProvider->backupPath() / url;
                  if (!exists(backup))
                     return boost::make_shared<CNoContentAnswer>();

                  boost::filesystem::remove(backup);
               }
               else
               {
                  // Delete all backups
                  for (boost::filesystem::directory_iterator backupDir(m_pathProvider->backupPath());
                       backupDir != boost::filesystem::directory_iterator();
                       ++backupDir)
                  {
                     if (!is_regular_file(backupDir->path()))
                        continue;

                     if (!boost::starts_with(backupDir->path().filename().string(), "backup_"))
                        continue;

                     boost::system::error_code ec;
                     boost::filesystem::remove(backupDir->path(), ec);
                  }
               }

               return boost::make_shared<CNoContentAnswer>();
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to delete backup");
            }
         }
      } //namespace service
   } //namespace rest
} //namespace web 
