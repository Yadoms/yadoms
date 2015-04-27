#pragma once
#include "source/IUpdateSource.h"
#include "task/Scheduler.h"

namespace update
{
   //-----------------------------------------------------------------------------
   /// \brief  Provide methods for starting update and check for updates
   //-----------------------------------------------------------------------------   
   class CUpdateManager
   {
   public:
      //-----------------------------------------------------------------------------
      /// \brief  Constructor
      /// \param [in]   taskScheduler        The task scheduler
      //-----------------------------------------------------------------------------   
      CUpdateManager(boost::shared_ptr<task::CScheduler> & taskScheduler);

      //-----------------------------------------------------------------------------
      /// \brief  Destructor
      //-----------------------------------------------------------------------------   
      virtual ~CUpdateManager();

      //-----------------------------------------------------------------------------
      /// \brief  Start a check for update (asynchronous; check for update result is provided by webservice)
      /// \param [in]   source        The source to check for update
      /// \return The check for update result (true if async request is made)
      //-----------------------------------------------------------------------------   
      bool checkForUpdateAsync(boost::shared_ptr<source::IUpdateSource> source);
      //-----------------------------------------------------------------------------
      /// \brief  Start an update (asynchronous; update result is provided by webservice)
      /// \param [in]   source        The source to update
      /// \return The update result (true if async request is made)
      //-----------------------------------------------------------------------------   
      bool updateAsync(boost::shared_ptr<source::IUpdateSource> source);

   private:
      //-----------------------------------------------------------------------------
      /// \brief  Start a task
      /// \param [in]   task        The task to start
      /// \return result (true/false)
      //-----------------------------------------------------------------------------   
      bool startTask(boost::shared_ptr<task::ITask> task);

   private:
      //-----------------------------------------------------------------------------
      /// \brief  Task scheduler
      //-----------------------------------------------------------------------------
      boost::shared_ptr<task::CScheduler> m_taskScheduler;
   };
   
} // namespace update
