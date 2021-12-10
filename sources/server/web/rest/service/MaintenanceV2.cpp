#include "stdafx.h"
#include <boost/date_time/c_local_time_adjustor.hpp>
#include "Maintenance.h"
#include "RestEndPoint.h"
#include "task/backup/ExportBackupHandler.h"
#include "task/exportAcquisitions/ExportAcquisitionsHandler.h"
#include "task/exportData/ExportData.h"
#include "task/exportLogs/ExportLogsHandler.h"
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

            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "maintenance/logs", getLogsPackageV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "maintenance/logs/{url}", getLogsPackageV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kPost, "maintenance/logs", createLogsPackageV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kDelete, "maintenance/logs", deleteLogsPackageV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kDelete, "maintenance/logs/{url}", deleteLogsPackageV2));

            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "maintenance/acquisitions", getAcquisitionsExportV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "maintenance/acquisitions/{url}", getAcquisitionsExportV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kPost, "maintenance/acquisitions", createAcquisitionsExportV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kDelete, "maintenance/acquisitions", deleteAcquisitionsExportV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kDelete, "maintenance/acquisitions/{url}", deleteAcquisitionsExportV2));


            //TODO            
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("restore")("*"), CMaintenance::restoreBackup)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("uploadBackup"), CMaintenance::uploadBackup)

            return m_endPoints;
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

         std::string CMaintenance::packLogsInProgress()
         {
            boost::lock_guard<boost::recursive_mutex> lock(m_packLogsInProgressTaskUidMutex);

            if (m_packLogsInProgressTaskUid.empty())
               return std::string();

            if (m_taskScheduler->getTask(m_packLogsInProgressTaskUid)->getStatus() != task::ETaskStatus::kStarted)
               m_packLogsInProgressTaskUid.clear();

            return m_packLogsInProgressTaskUid;
         }

         void CMaintenance::setPackLogsInProgress(const std::string& taskUid)
         {
            boost::lock_guard<boost::recursive_mutex> lock(m_packLogsInProgressTaskUidMutex);
            m_packLogsInProgressTaskUid = taskUid;
         }

         std::string CMaintenance::exportAcquisitionsInProgress()
         {
            boost::lock_guard<boost::recursive_mutex> lock(m_exportAcquisitionsInProgressTaskUidMutex);

            if (m_exportAcquisitionsInProgressTaskUid.empty())
               return std::string();

            if (m_taskScheduler->getTask(m_exportAcquisitionsInProgressTaskUid)->getStatus() != task::ETaskStatus::kStarted)
               m_exportAcquisitionsInProgressTaskUid.clear();

            return m_exportAcquisitionsInProgressTaskUid;
         }

         void CMaintenance::setExportAcquisitionsInProgressInProgress(const std::string& taskUid)
         {
            boost::lock_guard<boost::recursive_mutex> lock(m_exportAcquisitionsInProgressTaskUidMutex);
            m_exportAcquisitionsInProgressTaskUid = taskUid;
         }

         boost::shared_ptr<IAnswer> CMaintenance::getFilesPackage(const std::string& inputUrl,
                                                                  const std::string& packageFilePrefix,
                                                                  const std::function<std::string()>& checkInProgressFct,
                                                                  const std::string& resultArrayTag) const
         {
            try
            {
               const auto packagePath = m_pathProvider->backupPath();
               if (packagePath.empty() || !exists(packagePath) || !is_directory(packagePath))
                  return boost::make_shared<CNoContentAnswer>();

               shared::CDataContainer result;
               result.createArray(resultArrayTag);
               for (boost::filesystem::directory_iterator packageDir(packagePath); packageDir != boost::filesystem::directory_iterator(); ++
                    packageDir)
               {
                  if (!is_regular_file(packageDir->path()))
                     continue;

                  if (!boost::starts_with(packageDir->path().filename().string(), packageFilePrefix))
                     continue;

                  if (boost::iends_with(packageDir->path().filename().string(), ".inprogress"))
                     continue;

                  auto fileSize = file_size(packageDir->path());

                  const auto lastWriteTimeT = last_write_time(packageDir->path());
                  const auto lastWriteTimePosix = boost::date_time::c_local_adjustor<boost::posix_time::ptime>::utc_to_local(
                     boost::posix_time::from_time_t(lastWriteTimeT));

                  shared::CDataContainer file;
                  file.set("size", fileSize);
                  file.set("modificationDate", lastWriteTimePosix);
                  file.set("path", packageDir->path().string());
                  file.set("url", packageDir->path().filename().string());

                  if (!inputUrl.empty() && inputUrl == packageDir->path().filename().string())
                     return boost::make_shared<CSuccessAnswer>(file); // Get only one package

                  result.appendArray(resultArrayTag, file);
               }

               const auto packageInProgressTaskUid = checkInProgressFct();
               if (!packageInProgressTaskUid.empty())
                  result.set("inProgress", "tasks/" + packageInProgressTaskUid);

               if (result.arraySize(resultArrayTag) == 0 && packageInProgressTaskUid.empty())
                  return boost::make_shared<CNoContentAnswer>();

               return boost::make_shared<CSuccessAnswer>(result);
            }
            catch (const shared::exception::COutOfRange& exception)
            {
               YADOMS_LOG(error) << "Error processing getFilesPackage request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest);
            }
            catch (const std::exception& exception)
            {
               YADOMS_LOG(error) << "Error processing getFilesPackage request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get file package " + resultArrayTag);
            }
         }

         boost::shared_ptr<IAnswer> CMaintenance::createFilesPackage(const std::function<std::string()>& checkInProgressFct,
                                                                     const std::function<void(std::string)>& setInProgressFct,
                                                                     const std::function<boost::shared_ptr<task::ITask>()>& createTaskFct) const
         {
            try
            {
               if (!checkInProgressFct().empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "package already in progress");

               const auto task = createTaskFct();

               std::string taskUid;
               if (!m_taskScheduler->runTask(task, taskUid))
                  throw std::runtime_error("Task : " + task->getName() + " fail to start");
               setInProgressFct(taskUid);

               YADOMS_LOG(information) << "Task : " << task->getName() << " successfully started. TaskId = " << taskUid;

               return CHelpers::createLongRunningOperationAnswer(taskUid);
            }
            catch (const std::exception& exception)
            {
               YADOMS_LOG(error) << "Fail to create package : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to create package");
            }
         }

         boost::shared_ptr<IAnswer> CMaintenance::deleteFilesPackage(const std::string& inputUrl,
                                                                     const std::string& packageFilePrefix) const
         {
            try
            {
               if (!inputUrl.empty())
               {
                  // Delete one package
                  const auto package = m_pathProvider->backupPath() / inputUrl;
                  if (!exists(package))
                     return boost::make_shared<CNoContentAnswer>();

                  boost::filesystem::remove(package);
               }
               else
               {
                  // Delete all packages
                  for (boost::filesystem::directory_iterator packageDir(m_pathProvider->backupPath());
                       packageDir != boost::filesystem::directory_iterator();
                       ++packageDir)
                  {
                     if (!is_regular_file(packageDir->path()))
                        continue;

                     if (!boost::starts_with(packageDir->path().filename().string(), packageFilePrefix))
                        continue;

                     boost::system::error_code ec;
                     boost::filesystem::remove(packageDir->path(), ec);
                  }
               }

               return boost::make_shared<CNoContentAnswer>();
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to delete package");
            }
         }

         boost::shared_ptr<IAnswer> CMaintenance::getBackupsV2(const boost::shared_ptr<IRequest>& request)
         {
            if (!m_databaseRequester->backupSupported())
               return boost::make_shared<CNoContentAnswer>();

            return getFilesPackage(request->pathVariable("url", std::string()),
                                   "backup_",
                                   [this]() { return backupInProgress(); },
                                   "backups");
         }

         boost::shared_ptr<IAnswer> CMaintenance::createBackupsV2(const boost::shared_ptr<IRequest>& request)
         {
            if (!m_databaseRequester->backupSupported())
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                       "backup not supported");

            boost::lock_guard<boost::recursive_mutex> lock(m_backupInProgressTaskUidMutex);
            return createFilesPackage([this]() { return backupInProgress(); },
                                      [this](const auto& taskUid) { this->setBackupInProgress(taskUid); },
                                      [this]()
                                      {
                                         return boost::make_shared<task::exportData::CExportData>(
                                            m_pathProvider,
                                            std::make_unique<task::backup::CExportBackupHandler>(m_pathProvider,
                                                                                                 m_databaseRequester));
                                      });
         }

         boost::shared_ptr<IAnswer> CMaintenance::deleteBackupV2(const boost::shared_ptr<IRequest>& request) const
         {
            return deleteFilesPackage(request->pathVariable("url", std::string()),
                                      "backup_");
         }

         boost::shared_ptr<IAnswer> CMaintenance::getLogsPackageV2(const boost::shared_ptr<IRequest>& request)
         {
            return getFilesPackage(request->pathVariable("url", std::string()),
                                   "logs_",
                                   [this]() { return packLogsInProgress(); },
                                   "logs");
         }

         boost::shared_ptr<IAnswer> CMaintenance::createLogsPackageV2(const boost::shared_ptr<IRequest>& request)
         {
            boost::lock_guard<boost::recursive_mutex> lock(m_packLogsInProgressTaskUidMutex);
            return createFilesPackage([this]() { return packLogsInProgress(); },
                                      [this](const auto& taskUid) { this->setPackLogsInProgress(taskUid); },
                                      [this]()
                                      {
                                         return boost::make_shared<task::exportData::CExportData>(
                                            m_pathProvider,
                                            std::make_unique<task::exportLogs::CExportLogsHandler>(m_pathProvider));
                                      });
         }

         boost::shared_ptr<IAnswer> CMaintenance::deleteLogsPackageV2(const boost::shared_ptr<IRequest>& request) const
         {
            return deleteFilesPackage(request->pathVariable("url", std::string()),
                                      "logs_");
         }

         boost::shared_ptr<IAnswer> CMaintenance::getAcquisitionsExportV2(const boost::shared_ptr<IRequest>& request)
         {
            return getFilesPackage(request->pathVariable("url", std::string()),
                                   "acquisitions_",
                                   [this]() { return exportAcquisitionsInProgress(); },
                                   "acquisitions");
         }

         boost::shared_ptr<IAnswer> CMaintenance::createAcquisitionsExportV2(const boost::shared_ptr<IRequest>& request)
         {
            return nullptr;
            //TODO
            //boost::lock_guard<boost::recursive_mutex> lock(m_exportAcquisitionsInProgressTaskUidMutex);
            //return createFilesPackage([this]() { return exportAcquisitionsInProgress(); },
            //                          [this](const auto& taskUid) { this->setExportAcquisitionsInProgressInProgress(taskUid); },
            //                          [this]()
            //                          {
            //                             return boost::make_shared<task::exportData::CExportData>(
            //                                m_pathProvider,
            //                                std::make_unique<task::exportAcquisitions::CExportAcquisitionsHandler>(m_keywordRequester,
            //                                   m_acquisitionRequester,
            //                                   keywordId));
            //                          });
         }

         boost::shared_ptr<IAnswer> CMaintenance::deleteAcquisitionsExportV2(const boost::shared_ptr<IRequest>& request) const
         {
            return deleteFilesPackage(request->pathVariable("url", std::string()),
                                      "acquisitions_");
         }
      } //namespace service
   } //namespace rest
} //namespace web 
