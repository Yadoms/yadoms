#include "stdafx.h"
#include "Maintenance.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/RestDispatcher.h"
#include "web/rest/Result.h"
#include "task/backup/Backup.h"
#include <shared/Log.h>
#include <boost/date_time/c_local_time_adjustor.hpp>

namespace web
{
   namespace rest
   {
      namespace service
      {
         std::string CMaintenance::m_restKeyword = std::string("maintenance");

         CMaintenance::CMaintenance(boost::shared_ptr<const IPathProvider> pathProvider, boost::shared_ptr<database::IDatabaseRequester> databaseRequester, boost::shared_ptr<task::CScheduler> taskScheduler)
            : m_pathProvider(pathProvider), m_databaseRequester(databaseRequester), m_taskScheduler(taskScheduler)
         {
         }


         CMaintenance::~CMaintenance()
         {
         }


         void CMaintenance::configureDispatcher(CRestDispatcher& dispatcher)
         {
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("information"), CMaintenance::getDatabaseInformation);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("backup"), CMaintenance::getBackups);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("backup"), CMaintenance::startBackup);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "DELETE", (m_restKeyword)("backup")("*"), CMaintenance::deleteBackup);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "DELETE", (m_restKeyword)("backup"), CMaintenance::deleteAllBackups);
         }


         const std::string& CMaintenance::getRestKeyword()
         {
            return m_restKeyword;
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CMaintenance::getDatabaseInformation(const std::vector<std::string>& parameters, const std::string& requestContent) const
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
               return CResult::GenerateError("unknown exception in retreiving database information");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CMaintenance::startBackup(const std::vector<std::string>& parameters, const std::string& requestContent)
         {
            try
            {
               if (m_databaseRequester->backupSupported())
               {
                  boost::shared_ptr<task::ITask> task(boost::make_shared<task::backup::CBackup>(m_pathProvider, m_databaseRequester));

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

         boost::shared_ptr<shared::serialization::IDataSerializable> CMaintenance::getBackups(const std::vector<std::string>& parameters, const std::string& requestContent) const
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
                           auto filesize = boost::filesystem::file_size(i->path());

                           auto lastWriteTimeT = boost::filesystem::last_write_time(i->path());
                           auto lastWriteTimePosix = boost::date_time::c_local_adjustor<boost::posix_time::ptime>::utc_to_local(boost::posix_time::from_time_t(lastWriteTimeT));

                           shared::CDataContainer file;
                           file.set("size", filesize);
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
               return CResult::GenerateError("unknown exception in retreiving last backup data");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CMaintenance::deleteBackup(const std::vector<std::string>& parameters, const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 2)
               {
                  std::string urlToDelete = parameters[2];

                  auto backup = m_pathProvider->backupPath();
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

         boost::shared_ptr<shared::serialization::IDataSerializable> CMaintenance::deleteAllBackups(const std::vector<std::string>& parameters, const std::string& requestContent) const
         {
            try
            {
               auto backup = m_pathProvider->backupPath();

               boost::system::error_code ec;
               std::string errors = "";
               for (boost::filesystem::directory_iterator end_dir_it, it(backup); it != end_dir_it; ++it) 
               {
                  if (!boost::filesystem::remove_all(it->path(), ec))
                  {
                     errors += ec.message() + '\n';
                  }
               }
               if(errors.empty())
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
      } //namespace service
   } //namespace rest
} //namespace web 


