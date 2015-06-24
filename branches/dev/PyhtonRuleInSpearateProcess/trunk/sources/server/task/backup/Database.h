#pragma once
#include "task/ITask.h"
#include "server/database/IDataBackup.h"
#include <shared/event/EventHandler.hpp>

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
      // [END] ITask implementation

   private:
      //------------------------------------------
      ///\brief   Internal progress handler
      //------------------------------------------
      void OnProgressionUpdatedInternal(int remaining, int total, const std::string & message = "");

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
      ///\brief   The function pointer for reporting progression
      //------------------------------------------
      TaskProgressFunc m_reportRealProgress;
   };

} //namespace backup
} //namespace task
