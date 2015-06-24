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
         m_reportRealProgress(true, progression, message, shared::CStringExtension::EmptyString, shared::CDataContainer::EmptyContainer);
   }

   void CDatabase::doWork(TaskProgressFunc pFunctor)
   {
      m_reportRealProgress = pFunctor;
      m_dataBackupInterface->backupData(m_backupLocation, boost::bind(&CDatabase::OnProgressionUpdatedInternal, this, _1, _2, _3));
   }

} //namespace backup
} //namespace task
