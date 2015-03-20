#include "stdafx.h"

#include <shared/Log.h>
#include <shared/exception/Exception.hpp>
#include <shared/exception/InvalidParameter.hpp>
#include "server/database/IDataBackup.h"
#include "task/ITask.h"

#include "Database.h"
namespace task { namespace backup {

   std::string CDatabase::m_taskName = "system.databaseBackup";

   CDatabase::CDatabase(boost::shared_ptr< database::IDataBackup > dataBackupInterface, const std::string & backupLocation)
      :m_dataBackupInterface(dataBackupInterface), m_backupLocation(backupLocation)
   {
      if(!m_dataBackupInterface)
         throw shared::exception::CInvalidParameter("dataBackupInterface");
   }

   CDatabase::~CDatabase()
   {
   }

   const std::string & CDatabase::getName()
   {
      return m_taskName;
   }

   void CDatabase::OnProgressionUpdatedInternal(int remaining, int total, const std::string & message)
   {
      float progression = (float)(total - remaining)*(float)100.0 / (float)total;

      if(m_reportRealProgress)
         m_reportRealProgress(true, progression, message);
   }

   bool CDatabase::doWork(TaskProgressFunc pFunctor)
   {
      try
      {
         m_reportRealProgress = pFunctor;

         YADOMS_LOG(information) << "Start backup...";

         YADOMS_LOG(information) << "Backup data to :" << m_backupLocation;

         m_dataBackupInterface->backupData(m_backupLocation, boost::bind(&CDatabase::OnProgressionUpdatedInternal, this, _1, _2, _3));

         YADOMS_LOG(information) << "End of backup";
         return true;
      }
      catch(shared::exception::CException & ex)
      {
         YADOMS_LOG(error) << "Fail to backup database : " << ex.what();
      }
      catch(...)
      {
         YADOMS_LOG(error) << "Fail to backup database : unknow exception";
      }
      return false;
   }

} //namespace backup
} //namespace task
