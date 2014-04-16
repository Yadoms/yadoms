#pragma once
#include "task/ITask.h"
#include "server/database/IDataBackup.h"

namespace task { namespace backup {

   //------------------------------------------
   ///\brief   Database backup task
   //-----------------------------------------
   class CDatabase : public ITask
   {
   public:
      //------------------------------------------
      ///\brief   Constructor
      //------------------------------------------
      CDatabase(boost::shared_ptr< database::IDataBackup > dataBackupInterface, const std::string & backupLocation);

      //------------------------------------------
      ///\brief   Destructor
      //------------------------------------------
      virtual ~CDatabase();

   public:
      // ITask implementation
      virtual const std::string & getName();
      virtual void doWork(TaskProgressFunc pFunctor);
      // ITask implementation

   private:
      //------------------------------------------
      ///\brief   Interal progress handler
      //------------------------------------------
      void OnProgressionUpdatedInternal(int remaining, int total);

      //------------------------------------------
      ///\brief   The task name
      //------------------------------------------
      static std::string m_taskName;

      //------------------------------------------
      ///\brief   The backup data interface
      //------------------------------------------
      boost::shared_ptr<database::IDataBackup> m_dataBackupInterface;

      //------------------------------------------
      ///\brief   The backup location
      //------------------------------------------
      const std::string m_backupLocation;

      //------------------------------------------
      ///\brief   The function pointer for reporting prgression
      //------------------------------------------
      TaskProgressFunc m_reportRealProgress;
   };

} //namespace backup
} //namespace task
