#include <regex>

#include <boost/date_time/c_local_time_adjustor.hpp>
#include <shared/http/HttpHelpers.h>
#include <shared/tools/Filesystem.h>
#include "Maintenance.h"
#include "RestEndPoint.h"
#include "stdafx.h"
#include "task/IRunningTaskMutex.h"
#include "task/backup/ExportBackupHandler.h"
#include "task/backup/Restore.h"
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
            m_endPoints->push_back(MAKE_ENDPOINT(kPut, "maintenance/backups/{url}/restore", restoreBackupV2)); //TODO à tester
            m_endPoints->push_back(MAKE_ENDPOINT(kPost, "maintenance/backups/upload", uploadBackupV2));

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

            return m_endPoints;
         }

         boost::shared_ptr<IAnswer> CMaintenance::getFilesPackage(const std::string& inputUrl,
                                                                  const std::string& packageFilePrefix,
                                                                  const std::string& resultArrayTag,
                                                                  const boost::shared_ptr<task::IRunningTaskMutex>& taskInProgressHandler) const
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

               const auto packageInProgressTaskUid = taskInProgressHandler->inProgressTaskUid();
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

         boost::shared_ptr<IAnswer> CMaintenance::startNotReenteringTask(const boost::shared_ptr<task::IRunningTaskMutex>& taskInProgressHandler,
                                                                         const std::function<boost::shared_ptr<task::ITask>()>& taskFct) const
         {
            try
            {
               if (!taskInProgressHandler->inProgressTaskUid().empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "task already in progress");

               const auto task = taskFct();

               std::string taskUid;
               if (!m_taskScheduler->runTask(task, taskUid))
                  throw std::runtime_error("Task : " + task->getName() + " fail to start");
               taskInProgressHandler->setInProgressTaskUid(taskUid);

               YADOMS_LOG(information) << "Task : " << task->getName() << " successfully started. TaskId = " << taskUid;

               return CHelpers::createLongRunningOperationAnswer(taskUid);
            }
            catch (const std::exception& exception)
            {
               YADOMS_LOG(error) << "Fail to start task : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to start task");
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

         boost::shared_ptr<IAnswer> CMaintenance::getBackupsV2(const boost::shared_ptr<IRequest>& request) const
         {
            if (!m_databaseRequester->backupSupported())
               return boost::make_shared<CNoContentAnswer>();

            return getFilesPackage(request->pathVariable("url", std::string()),
                                   "backup_",
                                   "backups",
                                   m_backupInProgressTaskUidHandler);
         }

         boost::shared_ptr<IAnswer> CMaintenance::createBackupsV2(const boost::shared_ptr<IRequest>& request)
         {
            if (!m_databaseRequester->backupSupported())
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                       "backup not supported");

            return startNotReenteringTask(m_backupInProgressTaskUidHandler,
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

         boost::shared_ptr<IAnswer> CMaintenance::restoreBackupV2(const boost::shared_ptr<IRequest>& request)
         {
            if (!m_databaseRequester->backupSupported())
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                       "backup not supported");

            const auto url = request->pathVariable("url", std::string());

            return startNotReenteringTask(m_restoreBackupInProgressTaskUidHandler,
                                          [this, url]()
                                          {
                                             return boost::make_shared<task::backup::CRestore>(url,
                                                                                               m_pathProvider);
                                          });
         }

         boost::shared_ptr<IAnswer> CMaintenance::uploadBackupV2(const boost::shared_ptr<IRequest>& request) const
         {
            //TODO gérer en temps que tâche longue ?
            try
            {
               if (request->contentType() != EContentType::kMultipartFormData)
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kUnsupportedMediaType);

               const auto tempFile = shared::tools::CFilesystem::createTemporaryFilename();

               auto backupFilePartHandler = request->createFormDataPartFileHandler(tempFile);
               request->readParts({{"backup-file", backupFilePartHandler}});

               // Need to release backupFilePartHandler to move file
               const auto fileName = backupFilePartHandler->fileName();
               const auto fileSize = backupFilePartHandler->fileSize();
               backupFilePartHandler.reset();

               shared::tools::CFilesystem::rename(tempFile, m_pathProvider->backupPath() / fileName);

               YADOMS_LOG(information) << "Backup uploaded " << fileName << ", " << fileSize << "bytes";

               return boost::make_shared<CNoContentAnswer>();
            }
            catch (const std::exception& exception)
            {
               YADOMS_LOG(error) << "Error upload backup request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       std::string("Fail to upload backup file ") + exception.what());
            }
         }

         boost::shared_ptr<IAnswer> CMaintenance::getLogsPackageV2(const boost::shared_ptr<IRequest>& request) const
         {
            return getFilesPackage(request->pathVariable("url", std::string()),
                                   "logs_",
                                   "logs",
                                   m_packLogsInProgressTaskUidHandler);
         }

         boost::shared_ptr<IAnswer> CMaintenance::createLogsPackageV2(const boost::shared_ptr<IRequest>& request)
         {
            return startNotReenteringTask(m_packLogsInProgressTaskUidHandler,
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

         boost::shared_ptr<IAnswer> CMaintenance::getAcquisitionsExportV2(const boost::shared_ptr<IRequest>& request) const
         {
            return getFilesPackage(request->pathVariable("url", std::string()),
                                   "acquisitions_",
                                   "acquisitions",
                                   m_exportAcquisitionsInProgressTaskUidHandler);
         }

         boost::shared_ptr<IAnswer> CMaintenance::createAcquisitionsExportV2(const boost::shared_ptr<IRequest>& request)
         {
            if (request->contentType() != EContentType::kFormUrlEncoded)
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kUnsupportedMediaType);

            std::smatch result;
            const auto body = request->body();
            if (!std::regex_search(body, result, std::regex("^id=(\\d*)$")) || result.size() != 2)
               throw std::runtime_error("Invalid keyword ID");
            const auto keywordId = static_cast<int>(std::stol(shared::http::CHttpHelpers::urlDecode(result[1])));

            return startNotReenteringTask(m_exportAcquisitionsInProgressTaskUidHandler,
                                          [this, &keywordId]()
                                          {
                                             return boost::make_shared<task::exportData::CExportData>(
                                                m_pathProvider,
                                                std::make_unique<task::exportAcquisitions::CExportAcquisitionsHandler>(m_keywordRequester,
                                                   m_acquisitionRequester,
                                                   keywordId));
                                          });
         }

         boost::shared_ptr<IAnswer> CMaintenance::deleteAcquisitionsExportV2(const boost::shared_ptr<IRequest>& request) const
         {
            return deleteFilesPackage(request->pathVariable("url", std::string()),
                                      "acquisitions_");
         }
      } //namespace service
   } //namespace rest
} //namespace web 
