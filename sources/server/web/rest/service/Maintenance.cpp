#include "stdafx.h"
#include "Maintenance.h"
#include <shared/exception/NotImplemented.hpp>
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/RestDispatcher.h"
#include "web/rest/Result.h"
#include "task/backup/Database.h"
#include <shared/Log.h>

namespace web { namespace rest { namespace service {

   std::string CMaintenance::m_restKeyword= std::string("maintenance");

   CMaintenance::CMaintenance(boost::shared_ptr<database::IDatabaseRequester> databaseRequester, boost::shared_ptr<task::CScheduler> taskScheduler)
      :m_databaseRequester(databaseRequester), m_taskScheduler(taskScheduler)
   {
   }


   CMaintenance::~CMaintenance()
   {
   }


   void CMaintenance::configureDispatcher(CRestDispatcher & dispatcher)
   {
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("information"), CMaintenance::getDatabaseInformation);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("lastBackup"), CMaintenance::getLastDatabaseBackup);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("backup"), CMaintenance::startDatabaseBackup);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "DELETE", (m_restKeyword)("lastBackup"), CMaintenance::deleteLastDatabaseBackup);
   }


   const std::string & CMaintenance::getRestKeyword()
   {
      return m_restKeyword;
   }

   shared::CDataContainer CMaintenance::getDatabaseInformation(const std::vector<std::string> & parameters, const std::string & requestContent)
   {
      try
      {
         shared::CDataContainer result = m_databaseRequester->getInformation();
         result.set("backupSupport", m_databaseRequester->backupSupported());
         return CResult::GenerateSuccess(result);
      }
      catch (std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch (...)
      {
         return CResult::GenerateError("unknown exception in retreiving database information");
      }      
   }
   
   shared::CDataContainer CMaintenance::startDatabaseBackup(const std::vector<std::string> & parameters, const std::string & requestContent)
   {
      try
      {
         if (m_databaseRequester->backupSupported())
         {
            boost::shared_ptr<task::ITask> task(new task::backup::CDatabase(m_databaseRequester));

            std::string taskUid;
            if (m_taskScheduler->runTask(task, taskUid))
               YADOMS_LOG(information) << "Task : " << task->getName() << " successfully started. TaskId = " << taskUid;
            else
               YADOMS_LOG(error) << "Task : " << task->getName() << " fail to start";


            shared::CDataContainer result;
            result.set("taskId", taskUid);
            return web::rest::CResult::GenerateSuccess(result);
         }
         else
         {
            return web::rest::CResult::GenerateError("backup not supported");
         }
      }
      catch (std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch (...)
      {
         return CResult::GenerateError("unknown exception in starting database backup");
      }
      
   }

   shared::CDataContainer CMaintenance::getLastDatabaseBackup(const std::vector<std::string> & parameters, const std::string & requestContent)
   {
      try
      {
         if (m_databaseRequester->backupSupported())
         {
            auto backup = m_databaseRequester->lastBackupData();

            if (boost::filesystem::exists(backup))
            {
               auto filesize = boost::filesystem::file_size(backup);
            
               auto lastWriteTimeT = boost::filesystem::last_write_time(backup);
               auto lastWriteTimePosix = boost::posix_time::from_time_t(lastWriteTimeT);

               shared::CDataContainer result;
               result.set("size", filesize);
               result.set("modificationDate", lastWriteTimePosix);
               result.set("path", backup.string());
               result.set("url", backup.filename().string());
               return web::rest::CResult::GenerateSuccess(result);
            }
            else
            {
               //backup do not exists
               return web::rest::CResult::GenerateError();
            }
         }
         else
         {
            return web::rest::CResult::GenerateError("backup not supported");
         }
         
      }
      catch (std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch (...)
      {
         return CResult::GenerateError("unknown exception in retreiving last backup data");
      }

   }

   shared::CDataContainer CMaintenance::deleteLastDatabaseBackup(const std::vector<std::string> & parameters, const std::string & requestContent)
   {
      try
      {
         if (m_databaseRequester->backupSupported())
         {
            auto backup = m_databaseRequester->lastBackupData();
            if (boost::filesystem::exists(backup))
            {
               boost::system::error_code ec;
               if (boost::filesystem::remove(backup, ec))
                  return web::rest::CResult::GenerateSuccess();
               return web::rest::CResult::GenerateError(ec.message());
            }
            else
            {
               //backup do not exists
               return web::rest::CResult::GenerateError();
            }
         }
         else
         {
            return web::rest::CResult::GenerateError("backup not supported");
         }
      }
      catch (std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch (...)
      {
         return CResult::GenerateError("unknown exception in deleting last backup data");
      }

   }

} //namespace service
} //namespace rest
} //namespace web 
