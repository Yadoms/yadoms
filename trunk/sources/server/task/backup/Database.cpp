#include "stdafx.h"
#include "Database.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>
#include <shared/exception/InvalidParameter.hpp>

#include "server/database/IDataBackup.h"

namespace task { namespace backup {

   std::string CDatabase::m_taskName = "backup database task";

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

   void CDatabase::OnProgressionUpdatedInternal(int remaining, int total)
   {
      if(m_reportRealProgress)
         m_reportRealProgress( (float)(total-remaining)*(float)100.0 / (float)total);
   }

   void CDatabase::doWork(TaskProgressFunc pFunctor)
   {
      try
      {
         m_reportRealProgress = pFunctor;

         YADOMS_LOG(info) << "Start backup...";

         YADOMS_LOG(info) << "Backup data to :" << m_backupLocation;

         m_dataBackupInterface->backupData(m_backupLocation, boost::bind(&CDatabase::OnProgressionUpdatedInternal, this, _1, _2));

         YADOMS_LOG(info) << "End of backup";
      }
      catch(shared::exception::CException & ex)
      {
         YADOMS_LOG(error) << "Fail to backup database : " << ex.what();
      }
      catch(...)
      {
         YADOMS_LOG(error) << "Fail to backup database : unknow exception";
      }

   }

} //namespace backup
} //namespace task
